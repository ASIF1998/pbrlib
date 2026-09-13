#include <backend/renderer/vulkan/shader_compilers/shader_compiler.hpp>
#include <backend/renderer/vulkan/shader_compilers/glsl_shader_compiler.hpp>
#include <backend/renderer/vulkan/shader_compilers/slang_shader_compiler.hpp>

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
        const std::filesystem::path&    root_directory,
        std::span<const Define>         defines,
        bool                            dump
    )
    {
        if (const auto path_to_file = filename.string(); path_to_file.contains(".glsl"))
            return glsl::compile(device, filename, root_directory, defines, dump);

        return slang::compile(device, filename, root_directory, defines, dump);
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
