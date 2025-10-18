#include <backend/scene/mesh_manager.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>

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
    {
        _descriptor_set_layout_handle = vk::builders::DescriptorSetLayout(_device)
            .addBinding(Bindings::eVertexBuffers, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT)
            .addBinding(Bindings::eInstances, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        _descriptor_set_handle = _device.allocateDescriptorSet(_descriptor_set_layout_handle, "[mesh-manager] descriptor-set-layout");
    }

    void MeshManager::add (
        std::string_view                    name,
        std::span<const VertexAttribute>    attributes, 
        std::span<const uint32_t>           indices,
        SceneItem*                          ptr_item
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!ptr_item || !ptr_item->hasComponent<components::Renderable>()) [[unlikely]]
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

        const auto mesh_id = _instances[_item_to_instance_id[ptr_src_item]].mesh_id;

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

        if (_descriptor_set_is_changed) [[unlikely]]
        {
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

            _device.writeDescriptorSet ({
                .buffer     = _vbos_refs.value(),
                .set_handle = _descriptor_set_handle,
                .size       = static_cast<uint32_t>(_vbos_refs->size),
                .binding    = Bindings::eVertexBuffers
            });

            _device.writeDescriptorSet ({
                .buffer     = _instances_buffer.value(),
                .set_handle = _descriptor_set_handle,
                .size       = static_cast<uint32_t>(_instances_buffer->size),
                .binding    = Bindings::eInstances
            });

            _descriptor_set_is_changed = false;
        }

        _instances_buffer->write(std::span<const Instance>(_instances), 0);
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> MeshManager::descriptorSet() const noexcept
    {
        return std::make_pair(_descriptor_set_handle.handle(), _descriptor_set_layout_handle.handle());
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