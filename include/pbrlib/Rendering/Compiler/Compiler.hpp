#pragma once

#include <filesystem>

#include <vector>

#include <vulkan/vulkan.h>

namespace pbrlib::shader
{
    struct CompileResult
    {
        std::vector<uint32_t> IL;
        VkShaderStageFlagBits stage;
    };

    class GLSLCompiler
    {
    public:
        static CompileResult compile(std::filesystem::path filename);
    };
}