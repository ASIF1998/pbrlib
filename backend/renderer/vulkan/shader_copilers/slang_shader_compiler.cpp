#include <backend/renderer/vulkan/shader_copilers/slang_shader_compiler.hpp>

#include <slang.h>
#include <slang-com-ptr.h>
#include <slang-com-helper.h>

namespace pbrlib::backend::vk::shader::slang
{
    VkShaderModule compile(
        Device&                         device,
        const std::filesystem::path&    filename,
        std::span<const Define>         defines,
        bool                            dump
    )
    {
        Slang::ComPtr<::slang::IGlobalSession> slang_global_session;
        createGlobalSession(slang_global_session.writeRef());

        ::slang::TargetDesc target_desc = { };
        target_desc.format = SLANG_SPIRV;
        target_desc.profile = slang_global_session->findProfile("spirv_1_6");
        return VK_NULL_HANDLE;

        ::slang::SessionDesc session_desc = { };
        session_desc.targets = &target_desc;
        session_desc.targetCount = 1;

#if 0
        std::array macros
        {
            ::slang::PreprocessorMacroDesc("BIAS_VALUE", "1138"),
            ::slang::PreprocessorMacroDesc("OTHER_MACRO", "float")
        };

        session_desc.preprocessorMacros = macros.data();
        session_desc.preprocessorMacroCount = static_cast<SlangInt>(macros.size());
#endif

        Slang::ComPtr<::slang::ISession> session;
        slang_global_session->createSession(session_desc, session.writeRef());

        /// Load Modules
        /// Query Entry Points
        /// Compose Modules and Entry Points
        /// Get Target Kernel Code
        /// About Sessions
        /// Dynamic Dispatch
        /// Complete Example
    }
}
