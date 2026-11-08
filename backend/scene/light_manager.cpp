#include <backend/scene/light_manager.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <pbrlib/scene/scene.hpp>
#include <backend/components.hpp>
#include <pbrlib/components.hpp>

namespace pbrlib::backend
{
    LightManager::LightManager(vk::Device& device) :
        _device(device)
    {
        _descriptor_group.emplace(
            _device,
            vk::builders::DescriptorSetLayout(_device)
                .addBinding(Bindings::ePointLightBuffer, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL),
            "[light-manager] descriptor-set-group"
        );
    }

    void LightManager::add(SceneItem& scene_item, const math::vec3& color, float intensity)
    {
        const auto& transform = scene_item.getComponent<pbrlib::components::Transform>();

        auto& point_light = _point_lights.emplace_back();
        point_light.color       = color;
        point_light.intensity   = intensity;
        point_light.position_ws = math::vec3(transform.transform[0][3], transform.transform[1][3], transform.transform[2][3]);

        auto& component = scene_item.addComponent<components::PointLight>();
        component.light_id = _point_lights.size() - 1;

        _point_buffer_is_dirty = true;
    }

    void LightManager::update()
    {
        if (_point_lights.empty()) [[unlikely]]
            return;

        if (!_point_lights_buffer || _point_lights_buffer->size < _point_lights.size() * sizeof(pbrlib::gpu_cpu_interrop::PointLight))
        {
            _point_lights_buffer = vk::builders::Buffer(_device)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name("[light-manager] point-lights-buffer")
                .size(_point_lights.size() * sizeof(pbrlib::gpu_cpu_interrop::PointLight))
                .type(vk::BufferType::eDeviceOnly)
                .usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
                .build();

            _descriptor_group->write(vk::DescriptorBufferInfo
            {
                .buffer     = _point_lights_buffer.value(),
                .size       = static_cast<uint32_t>(_point_lights_buffer->size),
                .binding    = Bindings::ePointLightBuffer
            });
        }

        if (_point_buffer_is_dirty)
        {
            _point_lights_buffer->write(std::span<const pbrlib::gpu_cpu_interrop::PointLight>(_point_lights), 0);
            _point_buffer_is_dirty = false;
        }
    }
}
