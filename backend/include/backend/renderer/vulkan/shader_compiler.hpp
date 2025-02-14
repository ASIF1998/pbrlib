#pragma once

#include <vulkan/vulkan.h>

#include <filesystem>

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib::vk::shader
{
    void initCompiler();
    void finalizeCompiler();

    [[nodiscard]]
    VkShaderModule compile(const Device* ptr_device, const std::filesystem::path& filename);
}