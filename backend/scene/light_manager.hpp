#pragma once

#include "pbrlib/math/vec3.hpp"
#include <backend/shaders/lights.h>

#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/descriptor_group.hpp>

#include <vector>
#include <optional>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib::backend
{
    class LightManager final
    {
    public:
        struct Bindings
        {
            enum
            {
                ePointLightBuffer,

                eCount
            };
        };

        explicit LightManager(vk::Device& device);

        LightManager(LightManager&& light_manager)      = delete;
        LightManager(const LightManager& light_manager) = delete;

        LightManager& operator = (LightManager&& light_manager)         = delete;
        LightManager& operator = (const LightManager& light_manager)    = delete;

        void add(SceneItem& scene_item, const math::vec3& color, float intensity);

        void update();

    private:
        vk::Device& _device;

        std::vector<pbrlib::gpu_cpu_interrop::PointLight>   _point_lights;
        std::optional<vk::Buffer>                           _point_lights_buffer;

        std::optional<vk::DescriptorGroup> _descriptor_group;

        bool _point_buffer_is_dirty = false;
    };
}
