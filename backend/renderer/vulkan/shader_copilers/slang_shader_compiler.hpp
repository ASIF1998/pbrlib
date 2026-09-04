#pragma once

#include <backend/renderer/vulkan/shader_copilers/define.hpp>

#include <vulkan/vulkan.h>

#include <filesystem>

#include <span>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader::slang
{
    [[nodiscard]] VkShaderModule compile(
        Device&                         device, 
        const std::filesystem::path&    filename, 
        std::span<const Define>         defines,
        bool                            dump = false
    );
}
