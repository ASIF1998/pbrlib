#include "../utils.hpp"

#include <backend/renderer/frame_graph.hpp>
#include <backend/renderer/vulkan/device.hpp>

TEST(FrameGraphTests, Ctor)
{
    constexpr uint32_t width    = 800;
    constexpr uint32_t height   = 600;

    pbrlib::vk::Device device;
    device.init(nullptr);

    pbrlib::FrameGraph frame_graph (&device, width, height);

    EXPECT_EQ(width, frame_graph.size().width);
    EXPECT_EQ(height, frame_graph.size().height);
}