#pragma once

#include <backend/renderer/vulkan/shader_compilers/define.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

#include <filesystem>

#include <span>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader::glsl
{
    void init();
    void finalize();

    [[nodiscard]]
    vk::ShaderModuleHandle compile(
        const Device&                   device,
        const std::filesystem::path&    filename,
        const std::filesystem::path&    root_directory,
        std::span<const Define>         defines,
        bool                            dump = false
    );
}
