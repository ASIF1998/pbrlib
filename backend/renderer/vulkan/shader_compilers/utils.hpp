#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <string>
#include <span>
#include <filesystem>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader::utils
{
    [[nodiscard]] std::string               getSource(const std::filesystem::path& filename);
    void                                    dumpShader(std::span<const uint8_t> spv, const std::filesystem::path& filename);
    [[nodiscard]] vk::ShaderModuleHandle    createShaderModule(const Device& device, std::span<const uint8_t> spv);
}
