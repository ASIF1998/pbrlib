#include "../utils.hpp"

#include <backend/renderer/vulkan/shader_copilers/shader_compiler.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <pbrlib/event_system.hpp>
#include <backend/events.hpp>

#include <backend/utils/paths.hpp>
#include <backend/utils/scope_exit.hpp>

using namespace pbrlib;
using namespace pbrlib::backend;
using namespace pbrlib::backend::utils;

class ShaderCompiler :
    public ::testing::Test
{
public:
    void SetUp() override
    {
        if constexpr (!pbrlib::testing::vk::isSupport())
            GTEST_SKIP();

        EventSystem::emit(events::Initialize());
        device.emplace();
        device->init();
    }

    void TearDown() override
    {
        device = std::nullopt;
        EventSystem::emit(events::Finalize());
    }

    static std::optional<std::vector<uint8_t>> getFileSource(const std::filesystem::path& filename)
    {
        if (!std::filesystem::exists(filename)) [[unlikely]]
            return std::nullopt;

        if (std::ifstream file (filename, std::ios::binary); file) [[likely]]
        {
            file.seekg(0, std::ios::end);
            const auto size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<uint8_t> file_source (size);
            file.read(reinterpret_cast<char*>(file_source.data()), size);

            return file_source;
        }

        return std::nullopt;
    }

    void compareShaders(const std::filesystem::path& shader_name, const std::filesystem::path& reference_compiled_shader, const std::vector<backend::vk::shader::Define>& defines)
    {
        const auto shader_module = vk::shader::compile(*device, PBRLIB_ABS_PATH(shader_name), defines, true);

        const ScopeExit scope_destroy_shader_module([this, shader_module]
        {
            vkDestroyShaderModule(device->device(), shader_module, nullptr);
        });

        const auto compiled_shader_binary_filename = PBRLIB_ABS_PATH(shader_name) += ".spv";

        const auto compiled_shader_binary = getFileSource(compiled_shader_binary_filename);
        const auto reference_shader_binary = getFileSource(PBRLIB_ABS_PATH(reference_compiled_shader));
        if (compiled_shader_binary && reference_shader_binary) [[likely]]
        {
            pbrlib::testing::equality(compiled_shader_binary->size(), reference_shader_binary->size());

            const auto ptr_data_1 = compiled_shader_binary->data();
            const auto ptr_data_2 = reference_shader_binary->data();
            const auto size = reference_shader_binary->size();

            constexpr int memcpy_success_code = 0;
            pbrlib::testing::equality(std::memcmp(ptr_data_1, ptr_data_2, size), memcpy_success_code);
        }
        else
        {
            const auto msg = std::format("[shader-compiler-tests] failed load shaders binary: '{}' and '{}'", compiled_shader_binary_filename.string(), reference_compiled_shader.string());
            pbrlib::testing::thisTrue(false, msg);
        }

        if (std::filesystem::exists(compiled_shader_binary_filename)) [[likely]]
            std::filesystem::remove(compiled_shader_binary_filename);
    }

    std::optional<vk::Device> device;
};

class GlslCompilerTests : public ShaderCompiler
{ };

class SlangCompileTests : public ShaderCompiler
{ };

TEST_F(GlslCompilerTests, CompileShader)
{
    const std::vector<backend::vk::shader::Define> defines;

    EXPECT_NO_THROW({
        compareShaders("pbrlib-tests/renderer/shaders/simple_shader.glsl.comp", "pbrlib-tests/references/shaders/simple_shader.glsl.comp.spv", defines);
    });
}

TEST_F(GlslCompilerTests, CompileInvalidShader)
{
    const std::vector<backend::vk::shader::Define> defines;

    EXPECT_THROW({
        const auto shader_handle = vk::shader::compile(*device, PBRLIB_ABS_PATH("pbrlib-tests/renderer/shaders/invalid_shader.glsl.comp"), defines);
    }, exception::RuntimeError);
}

TEST_F(GlslCompilerTests, FileNotFound)
{
    const std::vector<backend::vk::shader::Define> defines;

    EXPECT_THROW({
        const auto shader_handle = vk::shader::compile(*device, PBRLIB_ABS_PATH("pbrlib-tests/renderer/shaders/non_existent_shader.glsl.comp"), defines);
    }, exception::InvalidState);
}

TEST_F(SlangCompileTests, CompileShader)
{
    const std::vector<backend::vk::shader::Define> defines;

    EXPECT_NO_THROW({
        compareShaders("pbrlib-tests/renderer/shaders/simple_shader.slang.comp", "pbrlib-tests/references/shaders/simple_shader.slang.comp.spv", defines);
    });
}
