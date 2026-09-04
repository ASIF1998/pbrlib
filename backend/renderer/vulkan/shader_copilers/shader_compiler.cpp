#include <backend/renderer/vulkan/shader_copilers/shader_compiler.hpp>
#include <backend/renderer/vulkan/shader_copilers/glsl_shader_compiler.hpp>
#include <backend/renderer/vulkan/shader_copilers/slang_shader_compiler.hpp>

namespace pbrlib::backend::vk::shader
{
    std::span<const VkSpecializationMapEntry> SpecializationInfoBase::entries() const noexcept
    {
        return _entries;
    }

    SpecializationInfoBase& SpecializationInfoBase::addEntry(uint32_t constant_id, uint32_t offset, size_t size)
    {
        _entries.emplace_back(constant_id, offset, size);
        return *this;
    }
}

namespace pbrlib::backend::vk::shader
{
    VkShaderModule compile(
        Device&                         device,
        const std::filesystem::path&    filename,
        std::span<const Define>         defines,
        bool                            dump
    )
    {
        if (const auto path_to_file = filename.string(); path_to_file.contains("glsl"))
            return glsl::compile(device, filename, defines, dump);

        return slang::compile(device, filename, defines, dump);
    }

    void initCompiler()
    {
        glsl::initCompiler();
    }

    void finalizeCompiler()
    {
        glsl::finalizeCompiler();
    }
}
