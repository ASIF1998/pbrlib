#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <pbrlib/config.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <backend/logger/logger.hpp>

namespace pbrlib
{
    FrameGraph::FrameGraph(vk::Device* ptr_device, const Config& config) :
        _ptr_device(ptr_device)
    {
        _image = vk::Image::Builder(_ptr_device)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .fillColor(pbrlib::math::vec3(0))
            .format(VK_FORMAT_R8G8B8A8_UNORM)
            .name("FrameGraph result image")
            .size(config.width, config.height)
            .tiling(VK_IMAGE_TILING_OPTIMAL)
            .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .build();
    }

    const vk::Image& FrameGraph::draw()
    {
        if (_ptr_render_pass)
            _ptr_render_pass->render(*_image);
        else 
            log::engine::error("[FrameGraph] Failed draw scene because render pass is empty");

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