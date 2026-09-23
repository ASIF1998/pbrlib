#include "../utils.hpp"
#include "../config.hpp"

#include <backend/renderer/vulkan/shader_compilers/shader_compiler.hpp>

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

    [[nodiscard]] static std::optional<std::vector<uint8_t>> getFileSource(const std::filesystem::path& filename)
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

    [[nodiscard]] static uint64_t fnv1a_hash(std::string_view str) noexcept
    {
        auto hash = 0xcbf29ce484222325ull;
        for (char c : str) {
            hash ^= static_cast<uint8_t>(c);
            hash *= 0x00000100000001b3ull;
        }

        return hash;
    }

    /**
     * @brief сomputes a deterministic 64-bit FNV-1a hash of a defines
     *
     * unlike std::hash, this implementation is guaranteed to yield identical
     * results across different compilers, standard libraries, and target
     * platforms, ensuring stable reference filenames during CI testing
     *
     * @param defines the span of shader defines to hash
     * @return deterministic 64-bit hash value
     */
    [[nodiscard]] static uint64_t calcHash(std::span<const backend::vk::shader::Define> defines) noexcept
    {
        auto hash = 0xcbf29ce484222325ull;
        for (const auto& define: defines)
        {
            hash ^= fnv1a_hash(define.name) ^ fnv1a_hash(define.value);
            hash *= 0x00000100000001b3ull;

        }

        return hash;
    }

    void checkShader(const std::filesystem::path& shader_name, std::span<const backend::vk::shader::Define> defines)
    {
        [[maybe_unused]] const auto shader_module = vk::shader::compile(*device, PBRLIB_ABS_PATH(shader_name), root_directory, defines, true);

        // Shader compilers (like Slang) can produce slightly different SPIR-V bytecode
        // across different operating systems. OS-specific prefixes prevent
        // byte-for-byte comparison failures on CI.
#ifdef PBRLIB_OS_WINDOWS
            const auto platform_prefix  = "windows-";
#elif PBRLIB_OS_APPLE
            const auto platform_prefix  = "apple-";
#elif PBRLIB_OS_LINUX
            const auto platform_prefix  = "linux-";
#else
    #error "Unsupported platform for shader testing"
#endif

        const auto references_directory         = PBRLIB_ABS_PATH("pbrlib-tests/references/shaders");
        const auto reference_compiled_shader    = references_directory / (platform_prefix + std::to_string(calcHash(defines)) + '-' + shader_name.filename().string() + ".spv");

        const auto compiled_shader_binary_filename = PBRLIB_ABS_PATH(shader_name) += ".spv";

        if constexpr (pbrlib::testing::generate_shaders_spvs)
            std::filesystem::copy_file(compiled_shader_binary_filename, reference_compiled_shader, std::filesystem::copy_options::overwrite_existing);

        const auto compiled_shader_binary   = getFileSource(compiled_shader_binary_filename);
        const auto reference_shader_binary  = getFileSource(PBRLIB_ABS_PATH(reference_compiled_shader));
        if (compiled_shader_binary && reference_shader_binary) [[likely]]
        {
            pbrlib::testing::equality(compiled_shader_binary->size(), reference_shader_binary->size());

            const auto ptr_data_1   = compiled_shader_binary->data();
            const auto ptr_data_2   = reference_shader_binary->data();
            const auto size         = reference_shader_binary->size();

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

    static const std::filesystem::path root_directory;
};

const std::filesystem::path ShaderCompiler::root_directory = PBRLIB_ABS_PATH("pbrlib-tests/renderer/shaders");

class GlslCompilerTests : public ShaderCompiler
{ };

class SlangCompilerTests : public ShaderCompiler
{ };

TEST_F(GlslCompilerTests, CompileComputeShader)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/simple_shader.glsl.comp", {});
    });
}

TEST_F(GlslCompilerTests, CompileVertexShader)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/simple_shader.glsl.vert", {});
    });
}

TEST_F(GlslCompilerTests, CompileFragmentShader)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/simple_shader.glsl.frag", {});
    });
}

TEST_F(GlslCompilerTests, CompileInvalidShader)
{
    EXPECT_THROW({
        [[maybe_unused]] const auto shader_handle = vk::shader::compile(*device, root_directory / "invalid_shader.glsl.comp", root_directory, {});
    }, exception::RuntimeError);
}

TEST_F(GlslCompilerTests, FileNotFound)
{
    EXPECT_THROW({
        [[maybe_unused]] const auto shader_handle = vk::shader::compile(*device, root_directory / "non_existent_shader.glsl.comp", root_directory, {});
    }, exception::InvalidState);
}

TEST_F(GlslCompilerTests, IncludeDirective)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/include_test.glsl.comp", {});
    });
}

TEST_F(GlslCompilerTests, Defines)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/define_test.glsl.comp", {});
    });

    EXPECT_NO_THROW({
        const std::vector define
        {
            backend::vk::shader::Define("PBRLIB_DEFINE_TEST", "")
        };

        checkShader("pbrlib-tests/renderer/shaders/define_test.glsl.comp", define);
    });
}

TEST_F(SlangCompilerTests, CompileComputeShader)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/simple_shader.slang.comp", {});
    });
}

TEST_F(SlangCompilerTests, CompileVertexShader)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/simple_shader.slang.vert", {});
    });
}

TEST_F(SlangCompilerTests, CompileFragmentShader)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/simple_shader.slang.frag", {});
    });
}

TEST_F(SlangCompilerTests, IncludeModule)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/module_test.slang.comp", {});
    });
}

TEST_F(SlangCompilerTests, Defines)
{
    EXPECT_NO_THROW({
        checkShader("pbrlib-tests/renderer/shaders/define_test.slang.comp", {});
    });

    EXPECT_NO_THROW({
        const std::vector define
        {
            backend::vk::shader::Define("PBRLIB_DEFINE_TEST", "")
        };

        checkShader("pbrlib-tests/renderer/shaders/define_test.slang.comp", define);
    });
}
