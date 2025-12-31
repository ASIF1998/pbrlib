#include <backend/renderer/vulkan/shader_compiler.hpp>
#include <backend/renderer/vulkan/check.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/logger/logger.hpp>

#include <backend/utils/paths.hpp>

#include <pbrlib/exceptions.hpp>

#include <glslang/Public/resource_limits_c.h>

#include <fstream>

#define CHECK(fn, log_fn, log_fn_arg)                                                               \
    do                                                                                              \
    {                                                                                               \
        if(!fn)                                                                                     \
            throw exception::RuntimeError(std::format("[shader-compiler] {}", log_fn(log_fn_arg))); \
    } while (false)

#define CHECK_SHADER(fn)    CHECK(fn, glslang_shader_get_info_log, ptr_shader)
#define CHECK_PROGRAM(fn)   CHECK(fn, glslang_program_get_info_log, ptr_program)

namespace pbrlib::backend::vk::shader::utils
{
    struct IncludeProcessData final
    {
        IncludeProcessData(std::string&& header_data, std::unique_ptr<glsl_include_result_t>&& ptr_include) :
            header_data         (std::move(header_data)),
            ptr_include_result  (std::move(ptr_include))
        { }

        IncludeProcessData(IncludeProcessData&& data)       = default;
        IncludeProcessData(const IncludeProcessData& data)  = delete;

        IncludeProcessData& operator = (IncludeProcessData&& data)      = default;
        IncludeProcessData& operator = (const IncludeProcessData& data) = delete;

        std::string                             header_data;
        std::unique_ptr<glsl_include_result_t>  ptr_include_result;
    };

    std::map<std::string, IncludeProcessData> includes_data;

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

    static glslang_stage_t getStage(const std::filesystem::path& filename)
    {
        const auto extension = filename.extension().string();

#define RETURN_STAGE(ext, vulkan_stage) if (std::strcmp(extension.data(), ext) == 0) return vulkan_stage

        RETURN_STAGE(".vert", GLSLANG_STAGE_VERTEX);
        RETURN_STAGE(".tesc", GLSLANG_STAGE_TESSCONTROL);
        RETURN_STAGE(".tesc", GLSLANG_STAGE_TESSEVALUATION);
        RETURN_STAGE(".geom", GLSLANG_STAGE_GEOMETRY);
        
        RETURN_STAGE(".task", GLSLANG_STAGE_TASK);
        RETURN_STAGE(".mesh", GLSLANG_STAGE_MESH);
        
        RETURN_STAGE(".frag", GLSLANG_STAGE_FRAGMENT);

        RETURN_STAGE(".comp", GLSLANG_STAGE_COMPUTE);
        
        RETURN_STAGE(".rgen", GLSLANG_STAGE_RAYGEN);
        RETURN_STAGE(".rint", GLSLANG_STAGE_INTERSECT);
        RETURN_STAGE(".rahit", GLSLANG_STAGE_ANYHIT);
        RETURN_STAGE(".rchit", GLSLANG_STAGE_CLOSESTHIT);
        RETURN_STAGE(".rmiss", GLSLANG_STAGE_MISS);
        RETURN_STAGE(".rcall", GLSLANG_STAGE_CALLABLE);

#undef RETURN_STAGE

        backend::log::error("[shader-compiler] undefined shader type: {}", filename.string());

        return GLSLANG_STAGE_COUNT;
    }

    static glsl_include_result_t* localInclude (
        void*       ctx, 
        const char* header_name, 
        const char* includer_name,
        size_t      include_depth
    )
    {
        backend::log::error("[shader-compiler] don't process local include files");
        return nullptr;
    }

    static glsl_include_result_t* systemInclude (
        void*       ctx, 
        const char* header_name, 
        const char* includer_name,
        size_t      include_depth
    )
    {
        if (!header_name) [[unlikely]]
            return nullptr;

        if (auto return_value = includes_data.find(header_name); return_value != std::end(includes_data))
            return return_value->second.ptr_include_result.get();
            
        static const auto src_root_directory = pbrlib::backend::utils::projectRoot() / "backend/shaders";

        std::string key     = header_name;
        std::string code    = getSource(src_root_directory / header_name);

        auto ptr_include_data = std::make_unique<glsl_include_result_t>();

        auto [iter, inserted] = includes_data.emplace (
            std::move(key),
            IncludeProcessData(std::move(code), std::move(ptr_include_data))
        );

        if (!inserted) [[unlikely]]
            throw exception::RuntimeError(std::format("[shader-compiler] failed to insert include file '{}': entry already exists in cache", header_name));

        iter->second.ptr_include_result->header_name   = iter->first.c_str();
        iter->second.ptr_include_result->header_data   = iter->second.header_data.c_str();
        iter->second.ptr_include_result->header_length = iter->second.header_data.size();

        return iter->second.ptr_include_result.get();
    }

    static int freeInclude(void* ctx, glsl_include_result_t* ptr_result)
    {
        if (ptr_result) [[likely]]
        {
            if (auto include_data = includes_data.find(ptr_result->header_name); include_data != std::end(includes_data)) [[likely]]
                includes_data.erase(include_data);
        }

        return 0;
    }
}

namespace pbrlib::backend::vk::shader
{
    std::vector<uint32_t> createIL(const std::filesystem::path& filename)
    {
        auto source = utils::getSource(filename);
        auto stage  = utils::getStage(filename);

        const glsl_include_callbacks_t includer 
        { 
            .include_system         = utils::systemInclude,
            .include_local          = utils::localInclude,
            .free_include_result    = utils::freeInclude
        };

        const glslang_input_t input 
        { 
            .language                           = GLSLANG_SOURCE_GLSL,
            .stage                              = stage,
            .client                             = GLSLANG_CLIENT_VULKAN,
            .client_version                     = GLSLANG_TARGET_VULKAN_1_3,
            .target_language                    = GLSLANG_TARGET_SPV,
            .target_language_version            = GLSLANG_TARGET_SPV_1_6,
            .code                               = source.c_str(),
            .default_version                    = 100,
            .default_profile                    = GLSLANG_NO_PROFILE,
            .force_default_version_and_profile  = false,
            .forward_compatible                 = false,
            .messages                           = GLSLANG_MSG_DEFAULT_BIT,
            .resource                           = glslang_default_resource(),
            .callbacks                          = includer
        };

        auto ptr_shader = glslang_shader_create(&input);

        CHECK_SHADER(glslang_shader_preprocess(ptr_shader, &input));
        CHECK_SHADER(glslang_shader_parse(ptr_shader, &input));

        auto ptr_program = glslang_program_create();
        glslang_program_add_shader(ptr_program, ptr_shader);

        CHECK_PROGRAM(glslang_program_link(ptr_program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT));

        glslang_program_SPIRV_generate(ptr_program, stage);

        if (auto spirv_message = glslang_program_SPIRV_get_messages(ptr_program)) [[unlikely]]
            throw exception::RuntimeError(std::format("[sahder-compiler]: {}", spirv_message));

        std::vector<uint32_t> il (glslang_program_SPIRV_get_size(ptr_program));

        glslang_program_SPIRV_get(ptr_program, il.data());

        glslang_program_delete(ptr_program);
        glslang_shader_delete(ptr_shader);

        return il;
    }

    VkShaderModule compile(const Device& device, const std::filesystem::path& filename)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        backend::log::info("[shader-compiler] compile shader: {}", filename.filename().string());

        auto il = createIL(backend::utils::projectRoot() / "backend" / filename);

        VkShaderModule shader_module_handle = VK_NULL_HANDLE;
        
        const VkShaderModuleCreateInfo shader_module_create_info 
        {
            .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize   = il.size() * sizeof(uint32_t),
            .pCode      = il.data()
        };

        VK_CHECK(vkCreateShaderModule(
            device.device(), 
            &shader_module_create_info, 
            nullptr, 
            &shader_module_handle
        ));

        return shader_module_handle;
    }    
}

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
    bool is_init = false;

    void initCompiler()
    {
        if (!glslang_initialize_process()) [[unlikely]]
            backend::log::error("[shader-compiler] failed initialize glslang.");
        
        is_init = true;
    }

    void finalizeCompiler()
    {
        if (is_init) [[likely]]
            glslang_finalize_process();

        is_init = false;
    }
}