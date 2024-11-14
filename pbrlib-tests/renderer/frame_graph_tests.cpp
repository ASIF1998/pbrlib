#include "../utils.hpp"

#include <backend/renderer/frame_graph.hpp>

TEST(FrameGraphTests, Ctor)
{
    constexpr uint32_t width    = 800;
    constexpr uint32_t height   = 600;

    pbrlib::FrameGraph frame_graph (width, height);

    EXPECT_EQ(width, frame_graph.size().width);
    EXPECT_EQ(height, frame_graph.size().height);
}