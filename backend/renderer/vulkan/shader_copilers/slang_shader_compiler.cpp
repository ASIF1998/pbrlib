#include <backend/renderer/vulkan/shader_copilers/slang_shader_compiler.hpp>
#include <pbrlib/exceptions.hpp>

#include <slang.h>
#include <slang-com-ptr.h>
#include <slang-com-helper.h>

#include <array>
#include <format>
#include <sstream>
#include <fstream>

namespace pbrlib::backend::vk::shader::slang
{
    /// @todo move to utils
    static std::string getSource(const std::filesystem::path& filename)
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

    VkShaderModule compile(
        Device&                         device,
        const std::filesystem::path&    filename,
        std::span<const Define>         defines,
        bool                            dump
    )
    {
        Slang::ComPtr<::slang::IGlobalSession> ptr_slang_global_session;
        createGlobalSession(ptr_slang_global_session.writeRef());

        ::slang::TargetDesc target_desc = { };
        target_desc.format = SLANG_SPIRV;
        target_desc.profile = ptr_slang_global_session->findProfile("spirv_1_6");

        constexpr std::array options
        {
            ::slang::CompilerOptionEntry(::slang::CompilerOptionName::EmitSpirvDirectly, {::slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr })
        };

#if 0
        std::array macros
        {
            ::slang::PreprocessorMacroDesc("BIAS_VALUE", "1138"),
            ::slang::PreprocessorMacroDesc("OTHER_MACRO", "float")
        };
#endif

        const ::slang::SessionDesc session_desc
        {
            .targets                    = &target_desc,
            .targetCount                = 1,
            .compilerOptionEntries      = options.data(),
            .compilerOptionEntryCount   = static_cast<uint32_t>(options.size())
#if 0
            .preprocessorMacros         = macros.data(),
            .preprocessorMacroCount     = static_cast<SlangInt>(macros.size())
#endif
        };


        Slang::ComPtr<::slang::ISession> ptr_session;
        ptr_slang_global_session->createSession(session_desc, ptr_session.writeRef());

        Slang::ComPtr<::slang::IModule> ptr_module;
        {
            Slang::ComPtr<::slang::IBlob> ptr_diagnostics_blob;

            const auto module_name = filename.root_name().string();
            const auto module_path = filename.string();
            const auto shader_source = getSource(filename);

            ptr_module = ptr_session->loadModuleFromSourceString(module_name.c_str(), module_path.c_str(), shader_source.c_str(), ptr_diagnostics_blob.writeRef());
            if (!ptr_module) [[unlikely]]
                throw exception::RuntimeError(std::format("[slang-shader-compiler] failed compile shader: {}", filename.string()));
        }

        Slang::ComPtr<::slang::IEntryPoint> ptr_entry_point;
        {
            Slang::ComPtr<::slang::IBlob> ptr_diagnostics_blob;
            ptr_module->findEntryPointByName("compute_main", ptr_entry_point.writeRef());

            if (!ptr_entry_point) [[unlikely]]
                throw exception::RuntimeError(std::format("[slang-shader-compiler] failed find entry point: {}", filename.string()));
        }

        const std::array<::slang::IComponentType*, 2> component_types { ptr_module, ptr_entry_point };

        Slang::ComPtr<::slang::IComponentType> ptr_composite_program;
        {
            Slang::ComPtr<::slang::IBlob> ptr_diagnostics_blob;
            const auto result = ptr_session->createCompositeComponentType(component_types.data(), component_types.size(), ptr_composite_program.writeRef(), ptr_diagnostics_blob.writeRef());
            if (result != SLANG_OK) [[unlikely]]
                throw exception::RuntimeError(std::format("[slang-shader-compiler] failed composite program: {}", filename.string()));

        }

        Slang::ComPtr<::slang::IComponentType> ptr_linked_program;
        {
            Slang::ComPtr<::slang::IBlob> ptr_diagnostics_blob;
            const auto result = ptr_composite_program->link(ptr_linked_program.writeRef(), ptr_diagnostics_blob.writeRef());
            // diagnoseIfNeeded(diagnosticsBlob);
            if (result != SLANG_OK) [[unlikely]]
                throw exception::RuntimeError(std::format("[slang-shader-compiler] failed link program: {}", filename.string()));
        }

        Slang::ComPtr<::slang::IBlob> ptr_spirv_code;
        {
            Slang::ComPtr<::slang::IBlob> ptr_diagnostics_blob;
            const auto result = ptr_linked_program->getEntryPointCode(0, 0, ptr_spirv_code.writeRef(), ptr_diagnostics_blob.writeRef());
            // diagnoseIfNeeded(diagnosticsBlob);
            if (result != SLANG_OK) [[unlikely]]
                throw exception::RuntimeError(std::format("[slang-shader-compiler] failed get spirv code: {}", filename.string()));
        }

        const auto spirv_code = ptr_spirv_code->getBufferPointer();

        /// @todo
        return VK_NULL_HANDLE;
    }
}
