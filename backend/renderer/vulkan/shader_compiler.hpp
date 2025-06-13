#pragma once

#include <vulkan/vulkan.h>

#include <filesystem>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader
{
    void initCompiler();
    void finalizeCompiler();

    [[nodiscard]]
    VkShaderModule compile(const Device& device, const std::filesystem::path& filename);
}