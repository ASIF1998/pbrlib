#include <backend/scene/mesh_manager.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/profiling.hpp>

#include <backend/components.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/exceptions.hpp>

#include <array>

namespace pbrlib::backend
{
    MeshManager::MeshManager(vk::Device& device) :
        _device (device)
    { }

    MeshManager::~MeshManager()
    {
        const auto device_handle = _device.device();

        vkDeviceWaitIdle(device_handle);
        vkDestroyDescriptorSetLayout(device_handle, _descriptor_set_layout_handle, nullptr);
        vkFreeDescriptorSets(device_handle, _device.descriptorPool(), 1, &_descriptor_set_handle);
    }

    void MeshManager::add (
        std::string_view                    name,
        std::span<const VertexAttribute>    attributes, 
        std::span<const uint32_t>           indices,
        SceneItem*                          ptr_item
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!ptr_item || !ptr_item->hasComponent<component::Renderable>())
            throw exception::InvalidArgument("[mesh-manager] ptr_item");

        auto& renderable = ptr_item->getComponent<component::Renderable>();

        renderable.mesh_id      = static_cast<uint32_t>(_vbos.size());
        renderable.vertex_count = attributes.size();
        renderable.index_count  = indices.size();

        constexpr VkFlags shared_buffer_usage = 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT
            |   VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

        _vbos.push_back (
            vk::Buffer::Builder(_device)
                .name(std::format("[vertex-buffer] {}", name))
                .addQueueFamilyIndex(_device.queue().family_index)
                .size(attributes.size_bytes())
                .usage(shared_buffer_usage | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
                .build()
        );

        _ibos.push_back (
            vk::Buffer::Builder(_device)
                .name(std::format("[index-buffer] {}", name))
                .addQueueFamilyIndex(_device.queue().family_index)
                .size(indices.size_bytes())
                .usage(shared_buffer_usage | VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
                .build()
        );

        _vbos.back().write(attributes, 0);
        _ibos.back().write(indices, 0);

        const auto& transform = ptr_item->getComponent<pbrlib::component::Transform>();

        const MeshDraw mesh_draw
        {
            .model  = transform.transform,
            .normal = math::transpose(pbrlib::math::inverse(transform.transform))
        };

        _meshes_draw_info.push_back(mesh_draw);

        _descriptor_set_is_changed = true;
    }

    void MeshManager::update()
    {
        if (_descriptor_set_is_changed)
        {
            if (_descriptor_set_handle == VK_NULL_HANDLE)
            {
                constexpr std::array bindings 
                {
                    VkDescriptorSetLayoutBinding
                    {
                        .binding            = Bindings::eVertexBuffersReferences,
                        .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                        .descriptorCount    = 1,
                        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT
                    },
                    VkDescriptorSetLayoutBinding
                    {
                        .binding            = Bindings::eMeshesDrawInfos,
                        .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                        .descriptorCount    = 1,
                        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT
                    }
                };

                const VkDescriptorSetLayoutCreateInfo desc_set_layout_create_info
                {
                    .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                    .bindingCount   = static_cast<uint32_t>(bindings.size()),
                    .pBindings      = bindings.data()
                };

                const auto device_handle = _device.device();
    
                VK_CHECK(vkCreateDescriptorSetLayout(
                    device_handle,
                    &desc_set_layout_create_info,
                    nullptr,
                    &_descriptor_set_layout_handle
                ));
    
                _descriptor_set_handle = _device.allocateDescriptorSet(_descriptor_set_layout_handle, "[mesh-manager] descriptor-set-layout");
            }

            std::vector<VkDeviceAddress> buffres_address;
            buffres_address.reserve(_vbos.size());

            for (const auto& buffer: _vbos)
                buffres_address.push_back(buffer.address());

            constexpr auto buffer_usage =
                    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
                |   VK_BUFFER_USAGE_TRANSFER_DST_BIT;

            _vbos_refs = vk::Buffer::Builder(_device)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name("[mesh-manager] vertex-buffers-refs")
                .size(_vbos.size() * sizeof(VkDeviceAddress))
                .type(vk::BufferType::eDeviceOnly)
                .usage(buffer_usage)
                .build();

            _meshes_draw_buffer = vk::Buffer::Builder(_device)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name("mesh-draw-data")
                .size(_meshes_draw_info.size() * sizeof(MeshDraw))
                .type(vk::BufferType::eDeviceOnly)
                .usage(buffer_usage)
                .build();

            _vbos_refs->write(std::span<const VkDeviceAddress>(buffres_address), 0);
            _meshes_draw_buffer->write(std::span<const MeshDraw>(_meshes_draw_info), 0);

            const VkDescriptorBufferInfo vbos_refs_info 
            { 
                .buffer = _vbos_refs->handle,
                .offset = 0,
                .range  = _vbos_refs->size
            };

            const VkDescriptorBufferInfo meshes_draw_info 
            { 
                .buffer = _meshes_draw_buffer->handle,
                .offset = 0,
                .range  = _meshes_draw_buffer->size
            };

            const std::array write_infos 
            {
                VkWriteDescriptorSet
                { 
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .dstSet             = _descriptor_set_handle,
                    .dstBinding         = 0,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .pBufferInfo        = &vbos_refs_info
                },
                VkWriteDescriptorSet 
                { 
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .dstSet             = _descriptor_set_handle,
                    .dstBinding         = 1,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .pBufferInfo        = &meshes_draw_info
                }
            };

            vkUpdateDescriptorSets(
                _device.device(),
                static_cast<uint32_t>(write_infos.size()), write_infos.data(),
                0, nullptr
            );

            _descriptor_set_is_changed = false;
        }
    }

    VkDescriptorSet MeshManager::descriptorSet() const
    {
        return _descriptor_set_handle;
    }

    const vk::Buffer& MeshManager::indexBuffer(uint32_t mesh_id) const
    {
        if (mesh_id >= _ibos.size())
            throw exception::InvalidArgument("[mesh-manager] invalid mesh id");

        return _ibos[mesh_id];
    }

    const vk::Buffer& MeshManager::vertexBuffer(uint32_t mesh_id) const
    {
        if (mesh_id >= _vbos.size())
            throw exception::InvalidArgument("[mesh-manager] invalid mesh id");

        return _vbos[mesh_id];
    }

    size_t MeshManager::meshCount() const
    {
        return _ibos.size();
    }
}