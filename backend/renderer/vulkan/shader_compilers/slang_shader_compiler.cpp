#include <backend/renderer/vulkan/shader_compilers/slang_shader_compiler.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <backend/profiling.hpp>

#include <backend/renderer/vulkan/shader_compilers/utils.hpp>

#include <pbrlib/exceptions.hpp>

#include <slang.h>
#include <slang-com-ptr.h>
#include <slang-com-helper.h>

#include <array>
#include <span>

namespace pbrlib::backend::vk::shader::slang
{
    VkShaderModule createShaderModule(Device& device, std::span<const uint8_t> spv)
    {
        VkShaderModule shader_module_handle = VK_NULL_HANDLE;

        const VkShaderModuleCreateInfo shader_module_create_info
        {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = spv.size_bytes(),
            .pCode = reinterpret_cast<const uint32_t*>(spv.data())
        };

        VK_CHECK(vkCreateShaderModule(
            device.device(),
            &shader_module_create_info,
            nullptr,
            &shader_module_handle
        ));

        return shader_module_handle;
    }

    void dumpShader(std::span<const uint8_t> spv, const std::filesystem::path& filename)
    {
        std::ofstream file (filename, std::ios::binary);
        if (file) [[likely]]
            file.write(reinterpret_cast<const char*>(spv.data()), spv.size_bytes());
    }

    void throwExcetion(const std::string_view msg, ::slang::IBlob* ptr_diagnostics_blob)
    {
        std::string throw_msg (msg);
        if (ptr_diagnostics_blob) [[likely]]
        {
            const auto ptr_to_msg   = reinterpret_cast<const char*>(ptr_diagnostics_blob->getBufferPointer());
            const auto size         = ptr_diagnostics_blob->getBufferSize();

            std::string_view diagnostic_msg (ptr_to_msg, size);

            throw_msg = std::format("{}\ndiagnostic:\n{}", throw_msg, diagnostic_msg);
        }

        throw exception::RuntimeError(throw_msg);
    }

    Slang::ComPtr<::slang::IModule> createModule(::slang::ISession* ptr_session, const std::filesystem::path& filename)
    {
        Slang::ComPtr<::slang::IBlob>   ptr_diagnostics_blob;
        Slang::ComPtr<::slang::IModule> ptr_module;

        const auto module_name      = filename.stem().string();
        const auto module_path      = filename.string();
        const auto shader_source    = utils::getSource(filename);

        ptr_module = ptr_session->loadModuleFromSourceString(
            module_name.c_str(),
            module_path.c_str(),
            shader_source.c_str(),
            ptr_diagnostics_blob.writeRef()
        );

        if (!ptr_module) [[unlikely]]
            throwExcetion(std::format("[slang-shader-compiler] failed compile shader: {}", filename.string()), ptr_diagnostics_blob);

        return ptr_module;
    }

    Slang::ComPtr<::slang::IEntryPoint> createEntryPoint(::slang::IModule* ptr_module, const std::filesystem::path& filename)
    {
        Slang::ComPtr<::slang::IEntryPoint> ptr_entry_point;
        ptr_module->findEntryPointByName("compute_main", ptr_entry_point.writeRef());

        if (!ptr_entry_point) [[unlikely]]
            throw exception::RuntimeError(std::format("[slang-shader-compiler] failed find entry point: {}", filename.string()));

        return ptr_entry_point;
    }

    Slang::ComPtr<::slang::IComponentType> createCompossiteProgram(
        ::slang::ISession*              ptr_session,
        ::slang::IModule*               ptr_module,
        ::slang::IEntryPoint*           ptr_entry_point,
        const std::filesystem::path&    filename
    )
    {
        const std::array<::slang::IComponentType*, 2> component_types { ptr_module, ptr_entry_point };

        Slang::ComPtr<::slang::IBlob>           ptr_diagnostics_blob;
        Slang::ComPtr<::slang::IComponentType>  ptr_composite_program;

        const auto result = ptr_session->createCompositeComponentType(component_types.data(), component_types.size(), ptr_composite_program.writeRef(), ptr_diagnostics_blob.writeRef());
        if (result != SLANG_OK) [[unlikely]]
            throwExcetion(std::format("[slang-shader-compiler] failed composite program: {}", filename.string()), ptr_diagnostics_blob);

        return ptr_composite_program;

    }

    Slang::ComPtr<::slang::IComponentType> createLinkedProgramm(::slang::IComponentType* ptr_composite_program, const std::filesystem::path& filename)
    {
        Slang::ComPtr<::slang::IBlob>           ptr_diagnostics_blob;
        Slang::ComPtr<::slang::IComponentType>  ptr_linked_program;

        const auto result = ptr_composite_program->link(ptr_linked_program.writeRef(), ptr_diagnostics_blob.writeRef());
        if (result != SLANG_OK) [[unlikely]]
            throwExcetion(std::format("[slang-shader-compiler] failed link program: {}", filename.string()), ptr_diagnostics_blob);

        return ptr_linked_program;
    }

    Slang::ComPtr<::slang::IBlob> getSpirv(::slang::IComponentType* ptr_linked_program, const std::filesystem::path& filename)
    {
        Slang::ComPtr<::slang::IBlob> ptr_spirv_code;
        Slang::ComPtr<::slang::IBlob> ptr_diagnostics_blob;

        const auto result = ptr_linked_program->getEntryPointCode(0, 0, ptr_spirv_code.writeRef(), ptr_diagnostics_blob.writeRef());
        if (result != SLANG_OK) [[unlikely]]
            throwExcetion(std::format("[slang-shader-compiler] failed get spirv code: {}", filename.string()), ptr_diagnostics_blob);

        return ptr_spirv_code;
    }

    VkShaderModule compile(
        Device&                         device,
        const std::filesystem::path&    filename,
        std::span<const Define>         defines,
        bool                            dump
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const ::SlangGlobalSessionDesc global_session_desc
        {
            .enableGLSL = true
        };

        Slang::ComPtr<::slang::IGlobalSession> ptr_slang_global_session;
        ::slang::createGlobalSession(&global_session_desc, ptr_slang_global_session.writeRef());

        const ::slang::TargetDesc target_desc
        {
            .format     = SLANG_SPIRV,
            .profile    = ptr_slang_global_session->findProfile("spirv_1_6")
        };

        constexpr std::array options
        {
            ::slang::CompilerOptionEntry(
                ::slang::CompilerOptionName::EmitSpirvDirectly,
                {::slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr }
            )
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

        auto ptr_module             = createModule(ptr_session, filename);
        auto ptr_entry_point        = createEntryPoint(ptr_module, filename);
        auto ptr_composite_program  = createCompossiteProgram(ptr_session, ptr_module, ptr_entry_point, filename);
        auto ptr_linked_program     = createLinkedProgramm(ptr_composite_program, filename);
        auto ptr_spirv_code         = getSpirv(ptr_linked_program, filename);

        std::span spv (reinterpret_cast<const uint8_t*>(ptr_spirv_code->getBufferPointer()), ptr_spirv_code->getBufferSize());

        if (dump) [[unlikely]]
            dumpShader(spv, std::filesystem::path(filename) += ".spv");

        return createShaderModule(device, spv);
    }
}
