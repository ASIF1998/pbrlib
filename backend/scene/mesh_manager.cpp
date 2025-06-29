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

        if (!ptr_item || !ptr_item->hasComponent<components::Renderable>())
            throw exception::InvalidArgument("[mesh-manager] ptr_item");

        auto& renderable = ptr_item->getComponent<components::Renderable>();

        renderable.instance_id  = static_cast<uint32_t>(_instances.size());
        renderable.vertex_count = attributes.size();
        renderable.index_count  = indices.size();

        constexpr VkFlags shared_buffer_usage = 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT
            |   VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

        _vbos.push_back (
            vk::builders::Buffer(_device)
                .name(std::format("[vertex-buffer] {}", name))
                .addQueueFamilyIndex(_device.queue().family_index)
                .size(attributes.size_bytes())
                .usage(shared_buffer_usage | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
                .build()
        );

        _ibos.push_back (
            vk::builders::Buffer(_device)
                .name(std::format("[index-buffer] {}", name))
                .addQueueFamilyIndex(_device.queue().family_index)
                .size(indices.size_bytes())
                .usage(shared_buffer_usage | VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
                .build()
        );

        _vbos.back().write(attributes, 0);
        _ibos.back().write(indices, 0);

        const auto& transform = ptr_item->getComponent<pbrlib::components::Transform>();

        const Instance instance
        {
            .model      = transform.transform,
            .normal     = math::transpose(math::inverse(transform.transform)),
            .mesh_id    = static_cast<uint32_t>(_vbos.size() - 1)
        };

        _item_to_instance_id.emplace(ptr_item, _instances.size());
        _instances.push_back(instance);

        _descriptor_set_is_changed = true;
    }

    void MeshManager::addInstance(const SceneItem* ptr_src_item, SceneItem* ptr_dst_item)
    {
        if (!ptr_src_item || !ptr_dst_item)
            throw exception::InvalidArgument("[mesh-manager] pointers to items is null");

        auto mesh_id = _instances[_item_to_instance_id[ptr_src_item]].mesh_id;

        const auto& transform = ptr_dst_item->getComponent<pbrlib::components::Transform>();

        const Instance dst_instance
        {
            .model      = transform.transform,
            .normal     = math::transpose(math::inverse(transform.transform)),
            .mesh_id    = mesh_id
        };

        const auto dst_instance_id = static_cast<uint32_t>(_instances.size());

        _item_to_instance_id.emplace(ptr_dst_item, dst_instance_id);
        _instances.push_back(dst_instance);

        const auto& src_renderable = ptr_src_item->getComponent<components::Renderable>();
        auto&       dst_renderable = ptr_dst_item->getComponent<components::Renderable>();

        dst_renderable = src_renderable;

        dst_renderable.instance_id  = dst_instance_id;
        dst_renderable.ptr_item     = ptr_dst_item;

        _descriptor_set_is_changed = true;
    }

    void MeshManager::update()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (_descriptor_set_is_changed)
        {
            if (_descriptor_set_handle == VK_NULL_HANDLE)
            {
                constexpr std::array bindings 
                {
                    VkDescriptorSetLayoutBinding
                    {
                        .binding            = Bindings::eVertexBuffers,
                        .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                        .descriptorCount    = 1,
                        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT
                    },
                    VkDescriptorSetLayoutBinding
                    {
                        .binding            = Bindings::eInstances,
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

            _vbos_refs = vk::builders::Buffer(_device)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name("[mesh-manager] vertex-buffers-refs")
                .size(_vbos.size() * sizeof(VkDeviceAddress))
                .type(vk::BufferType::eDeviceOnly)
                .usage(buffer_usage)
                .build();

            _instances_buffer = vk::builders::Buffer(_device)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name("instances")
                .size(_instances.size() * sizeof(Instance))
                .type(vk::BufferType::eDeviceOnly)
                .usage(buffer_usage)
                .build();

            _vbos_refs->write(std::span<const VkDeviceAddress>(buffres_address), 0);

            const VkDescriptorBufferInfo vbos_refs_info 
            { 
                .buffer = _vbos_refs->handle,
                .offset = 0,
                .range  = _vbos_refs->size
            };

            const VkDescriptorBufferInfo meshes_draw_info 
            { 
                .buffer = _instances_buffer->handle,
                .offset = 0,
                .range  = _instances_buffer->size
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

        _instances_buffer->write(std::span<const Instance>(_instances), 0);
    }

    VkDescriptorSet MeshManager::descriptorSet() const noexcept
    {
        return _descriptor_set_handle;
    }

    const vk::Buffer& MeshManager::indexBuffer(uint32_t instance_id) const
    {
        if (instance_id >= _instances.size())
            throw exception::InvalidArgument("[mesh-manager] invalid instance id");

        return _ibos[_instances[instance_id].mesh_id];
    }

    const vk::Buffer& MeshManager::vertexBuffer(uint32_t instance_id) const
    {
        if (instance_id >= _instances.size())
            throw exception::InvalidArgument("[mesh-manager] invalid instance id");

        return _vbos[_instances[instance_id].mesh_id];
    }

    size_t MeshManager::meshCount() const noexcept
    {
        return _ibos.size();
    }

    void MeshManager::updateItemTransform(const SceneItem* ptr_item, const math::mat4& transform)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (auto index = _item_to_instance_id.find(ptr_item); index != std::end(_item_to_instance_id))
        {
            auto& instance      = _instances[index->second];
            instance.model     = transform;
            instance.normal    = math::transpose(math::inverse(transform));        
        }
    }
}