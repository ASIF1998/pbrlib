#include "utils.hpp"

#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

TEST(EngineTests, PreRenderCallback)
{
    if constexpr (!pbrlib::testing::vk::isSupport())
        GTEST_SKIP() << "skipped: vulkan support is not available on this platform";

    pbrlib::Config config;
    config.title            = "engine-pre-render-callback-test";
    config.draw_in_window   = false;

    pbrlib::Engine engine (config);

    int value = 0;
    engine.preRenderCallback([&value]
    {
        value = 123;
    });

    engine.run();

    pbrlib::testing::equality(value, 123);
}

TEST(EngineTests, PostRenderCallback)
{
    if constexpr (!pbrlib::testing::vk::isSupport())
        GTEST_SKIP() << "skipped: vulkan support is not available on this platform";

    pbrlib::Config config;
    config.title            = "engine-post-render-callback-test";
    config.draw_in_window   = false;

    pbrlib::Engine engine (config);

    int value = 0;
    engine.postRenderCallback([&value]
    {
        value = 321;
    });

    engine.run();

    pbrlib::testing::equality(value, 321);
}

TEST(EngineTests, PresentToDisplayCallback)
{
    if constexpr (!pbrlib::testing::vk::isSupport())
        GTEST_SKIP() << "skipped: vulkan support is not available on this platform";

    pbrlib::Config config;
    config.title            = "engine-present-to-display-callback-test";
    config.draw_in_window   = false;

    pbrlib::Engine engine (config);

    int value = 0;
    engine.postRenderCallback([&value]
    {
        value = 46362;
    });

    engine.run();

    pbrlib::testing::equality(value, 46362);
}