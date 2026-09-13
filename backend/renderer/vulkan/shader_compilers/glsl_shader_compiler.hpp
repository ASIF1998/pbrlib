#pragma once

#include <backend/renderer/vulkan/shader_compilers/define.hpp>

#include <vulkan/vulkan.h>

#include <filesystem>

#include <span>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader::glsl
{
    void initCompiler();
    void finalizeCompiler();

    [[nodiscard]]
    VkShaderModule compile(
        const Device&                   device,
        const std::filesystem::path&    filename,
        const std::filesystem::path&    root_directory,
        std::span<const Define>         defines,
        bool                            dump = false
    );
}
