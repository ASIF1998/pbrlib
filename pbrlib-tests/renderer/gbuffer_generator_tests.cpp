#include "../utils.hpp"
#include "../image_comparison/image_comparison.hpp"

#include "framegraph_resources_getter.hpp"

#include <backend/utils/paths.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/frame_graph/gbuffer_generator.hpp>

#include <pbrlib/config.hpp>
#include <pbrlib/scene/scene.hpp>
#include <pbrlib/engine.hpp>

#include <optional>

#include <filesystem>

class GBufferGeneratorTests :
    public ::testing::Test
{
public:
    void SetUp() override
    {
        if constexpr (!pbrlib::testing::vk::isSupport())
            GTEST_SKIP();

        _config =
        {
            .title          = "gbuffer-generator-tests",
            .width          = 800,
            .height         = 600,
            .draw_in_window = false
        };

        _engine.emplace(_config);
        _frame_graph_getter.emplace(_engine.value());
        _comparator.emplace(_frame_graph_getter->device());
    }

    void TearDown() override 
    {
        _frame_graph_getter = std::nullopt;
        _comparator         = std::nullopt;
        _engine             = std::nullopt;
    }

    void setup(const std::filesystem::path& content)
    {
        _engine->setupCallback([this, content] (pbrlib::Engine& engine, pbrlib::Scene& scene)
        {
            auto& camera = engine.camera();
            camera.up(pbrlib::math::vec3(0, -1, 0));
            camera.eye(pbrlib::math::vec3(-5, 5, 16.0));
            camera.fovY(60.0f);
            camera.range(0.01f, 1000.0f);
            camera.width(_config.width);
            camera.height(_config.height);
            
            pbrlib::testing::thisTrue(scene.import(pbrlib::backend::utils::projectRoot() / "pbrlib-tests/content" / content.string(), engine));
        });
    }

    [[nodiscard]]
    void check(const std::filesystem::path& reference_image, std::string_view attachment_name)
    {
        static const auto refs_root_dir = pbrlib::backend::utils::projectRoot() / "pbrlib-tests/references";

        _engine->run();

        auto ptr_result = _frame_graph_getter->image(attachment_name);

        ptr_result->changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        pbrlib::testing::thisTrue(_comparator->compare(*ptr_result, refs_root_dir / reference_image));
    }

private:
    std::optional<pbrlib::Engine>                   _engine;
    std::optional<pbrlib::testing::ImageComparison> _comparator;

    pbrlib::Config _config;

    std::optional<pbrlib::testing::FrameGraphResourcesGetter> _frame_graph_getter;
};

TEST_F(GBufferGeneratorTests, JunkShopAttachments)
{
    /// @fixme fix bug with insufficient precision when saving to file

    // setup("Blender 2.glb");
    // check("gbuffer_generator/junk-shop-attachment-normal-tangent.png", pbrlib::GBufferFinalAttachmentsName::normal_tangent);
    // check("gbuffer_generator/junk-shop-attachment-pos-uv.png", pbrlib::GBufferFinalAttachmentsName::pos_uv);
}