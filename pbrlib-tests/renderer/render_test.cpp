#include "render_test.hpp"

namespace pbrlib::testing
{
    RenderTest::RenderTest(std::string_view name, uint32_t width, uint32_t height)
    {
        _config.title           = name;
        _config.width           = width;
        _config.height          = height;
        _config.draw_in_window  = false;
    }

    void RenderTest::SetUp()
    {
        if constexpr (!pbrlib::testing::vk::isSupport())
            GTEST_SKIP() << "fix bug with insufficient precision when saving to file";

        _engine.emplace(_config);
        _frame_graph_getter.emplace(_engine.value());
        _comparator.emplace(_frame_graph_getter->device());
    }

    void RenderTest::TearDown() 
    {
        _frame_graph_getter = std::nullopt;
        _comparator         = std::nullopt;
        _engine             = std::nullopt;
    }

    void RenderTest::setup(const std::filesystem::path& content, const Settings& settings)
    {
        _engine->setup([this, content, &settings] (pbrlib::Scene& scene)
        {
            auto& camera = _engine->camera();
            camera.up(settings.up);
            camera.eye(settings.eye);
            camera.pos(settings.pos);
            camera.fovY(settings.fov);
            camera.range(0.01f, 100.0f);
            camera.width(_config.width);
            camera.height(_config.height);
            
            pbrlib::testing::thisTrue(scene.import(*_engine, pbrlib::backend::utils::projectRoot() / "pbrlib-tests/content" / content.string()));
        });
    }

    void RenderTest::check(const std::filesystem::path& reference_image, std::string_view attachment_name)
    {
        static const auto refs_root_dir = pbrlib::backend::utils::projectRoot() / "pbrlib-tests/references";

        backend::log::info("[render-test] starting render for {}", reference_image.filename().string());

        _engine->run();

        auto ptr_result = _frame_graph_getter->image(attachment_name);

        ptr_result->changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        pbrlib::testing::thisTrue(_comparator->compare(*ptr_result, refs_root_dir / reference_image));
    }

    pbrlib::Config& RenderTest::config() noexcept
    {
        return _config;
    }
    
    pbrlib::Engine& RenderTest::engine() noexcept
    {
        return _engine.value();
    }
}