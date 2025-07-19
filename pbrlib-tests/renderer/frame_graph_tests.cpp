#include "../utils.hpp"

#include <pbrlib/config.hpp>

#include <backend/renderer/frame_graph/frame_graph.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/renderer/canvas.hpp>

#include <backend/scene/material_manager.hpp>
#include <backend/scene/mesh_manager.hpp>

TEST(FrameGraphTests, Ctor)
{
    if constexpr (!pbrlib::testing::vk::isSupport())
        GTEST_SKIP();

    ASSERT_NO_THROW([]
    {
        constexpr uint32_t width    = 800;
        constexpr uint32_t height   = 600;

        pbrlib::Config config;
        config.width            = width;
        config.height           = height;
        config.draw_in_window   = false;

        pbrlib::backend::vk::Device device;
        device.init();

        pbrlib::backend::Canvas canvas(device, config.width, config.height);
        
        pbrlib::backend::MaterialManager    material_manager    (device);
        pbrlib::backend::MeshManager        mesh_manager        (device);

        pbrlib::backend::FrameGraph frame_graph(device, canvas, material_manager, mesh_manager);
    });
}