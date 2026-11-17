#include "../utils.hpp"

#include <pbrlib/config.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/canvas.hpp>

#include <backend/utils/align_size.hpp>

#include <backend/shaders/gpu_cpu_constants.h>

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
            .width          = width,
            .height         = height,
            .draw_in_window = false
        };

        _device.init();
        _canvas.emplace(_device, config.width, config.height);
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

    constexpr auto expected_width   = pbrlib::backend::utils::alignSize<uint32_t>(canvas_width, PBRLIB_WORK_GROUP_SIZE);
    constexpr auto expected_height  = pbrlib::backend::utils::alignSize<uint32_t>(canvas_height, PBRLIB_WORK_GROUP_SIZE);

    pbrlib::testing::equality(width, expected_width);
    pbrlib::testing::equality(height, expected_height);
}
