#include <backend/renderer/vulkan/shader_compilers/utils.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <pbrlib/exceptions.hpp>

#include <fstream>
#include <sstream>

#include <format>

namespace pbrlib::backend::vk::shader::utils
{
    std::string getSource(const std::filesystem::path& filename)
    {
        if (!std::filesystem::exists(filename)) [[unlikely]]
            throw exception::InvalidState(std::format("[shader-compiler] not find file: {}", filename.string()));

        std::ifstream file(filename);

        if (!file) [[unlikely]]
            throw exception::FileOpen(std::format("[shader-compiler] {}", filename.string()));

        std::ostringstream contents;
        contents << file.rdbuf();

        return contents.str();
    }

    void dumpShader(std::span<const uint8_t> spv, const std::filesystem::path& filename)
    {
        std::ofstream file (filename, std::ios::binary);
        if (file) [[likely]]
            file.write(reinterpret_cast<const char*>(spv.data()), spv.size_bytes());
    }

    vk::ShaderModuleHandle createShaderModule(const Device& device, std::span<const uint8_t> spv)
    {
        VkShaderModule shader_module_handle = VK_NULL_HANDLE;

        const VkShaderModuleCreateInfo shader_module_create_info
        {
            .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize   = spv.size_bytes(),
            .pCode      = reinterpret_cast<const uint32_t*>(spv.data())
        };

        VK_CHECK(vkCreateShaderModule(
            device.device(),
            &shader_module_create_info,
            nullptr,
            &shader_module_handle
        ));

        return vk::ShaderModuleHandle(shader_module_handle);
    }
}
