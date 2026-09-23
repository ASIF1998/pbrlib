#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <filesystem>

#include <span>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader
{
    struct Define;
}

namespace pbrlib::backend::vk::shader::slang
{
    [[nodiscard]] vk::ShaderModuleHandle compile(
        Device&                         device,
        const std::filesystem::path&    filename,
        const std::filesystem::path&    root_directory,
        std::span<const Define>         defines,
        bool                            dump = false
    );
}
