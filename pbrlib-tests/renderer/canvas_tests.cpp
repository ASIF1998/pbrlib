#include "../utils.hpp"

#include <pbrlib/config.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/canvas.hpp>

class CanvasTests :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        if constexpr (!pbrlib::testing::vk::isSupport())
            GTEST_SKIP();        
    }

    void setup(uint32_t width, uint32_t height)
    {
        const pbrlib::Config config
        {
            .title          = "canvas-tests",
            .width          = 100, 
            .height         = 200,
            .draw_in_window = false
        };

        _device.init();
        _canvas.emplace(_device, nullptr, config);
    }

    pbrlib::backend::vk::Device _device;

    std::optional<pbrlib::backend::Canvas> _canvas;
};

TEST_F(CanvasTests, Ctor)
{
    constexpr uint32_t canvas_width    = 100;
    constexpr uint32_t canvas_height   = 200;

    setup(canvas_width, canvas_height);

    const auto [width, height] = _canvas->size();

    pbrlib::testing::equality(width, canvas_width);
    pbrlib::testing::equality(height, canvas_height);
}