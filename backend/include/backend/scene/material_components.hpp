#pragma once

#include <backend/renderer/vulkan/image.hpp>

namespace pbrlib
{
    struct PbrMaterialComponent
    {
        vk::Image albedo;
        vk::Image normal_map;
        vk::Image metallic;
        vk::Image roughness;
    };
}