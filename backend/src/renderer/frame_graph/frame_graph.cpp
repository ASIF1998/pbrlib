#include <pbrlib/rendering/window.hpp>

#include <backend/renderer/frame_graph/frame_graph.hpp>
#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/device.hpp>

namespace pbrlib
{
    FrameGraph::FrameGraph(vk::Device* ptr_device, uint32_t width, uint32_t height) :
        _ptr_device(ptr_device)
    {
        if (!ptr_device) 
            throw std::runtime_error("[FrameGraph] Pointer to device is null");

        _image = vk::Image::Builder(_ptr_device)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .fillColor(pbrlib::math::vec3(0))
            .format(VK_FORMAT_R8G8B8A8_UNORM)
            .name("FrameGraph result image")
            .size(width, height)
            .tiling(VK_IMAGE_TILING_OPTIMAL)
            .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .build();

    }

    FrameGraph::FrameGraph(vk::Device* ptr_device, const Window* ptr_window) :
        _ptr_device (ptr_device)
    {
        if (!ptr_device) 
            throw std::runtime_error("[FrameGraph] Pointer to device is null");

        const auto [width, height] = ptr_window->size();

        _image = vk::Image::Builder(_ptr_device)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .fillColor(pbrlib::math::vec3(0))
            .format(VK_FORMAT_R8G8B8_UNORM)
            .name("FrameGraph result image")
            .size(width, height)
            .tiling(VK_IMAGE_TILING_OPTIMAL)
            .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .build();
    }

    const vk::Image& FrameGraph::draw()
    {
        return *_image;
    }

    const Size FrameGraph::size() const
    {
        return Size
        {
            .width  = _image->width,
            .height = _image->height
        };
    }
}