#include "../utils.hpp"

#include <pbrlib/config.hpp>

#include <backend/renderer/frame_graph/frame_graph.hpp>
#include <backend/renderer/vulkan/device.hpp>

TEST(FrameGraphTests, Ctor)
{
    constexpr uint32_t width    = 800;
    constexpr uint32_t height   = 600;

    const pbrlib::Config config
    {
        .width          = width,
        .height         = height,
        .drawInWindow   = false
    };

    pbrlib::vk::Device device;
    device.init();

    pbrlib::FrameGraph frame_graph (&device, config);

    EXPECT_EQ(width, frame_graph.size().width);
    EXPECT_EQ(height, frame_graph.size().height);
}