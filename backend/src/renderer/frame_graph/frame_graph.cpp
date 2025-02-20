#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <backend/utils/vulkan.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/renderer/frame_graph/gbuffer_generator.hpp>

#include <backend/logger/logger.hpp>

#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>

namespace pbrlib
{
    FrameGraph::FrameGraph(vk::Device* ptr_device, const Config& config) :
        _ptr_device(ptr_device)
    {
        _image = vk::Image::Builder(_ptr_device)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .fillColor(math::vec3(0))
            .format(VK_FORMAT_R8G8B8A8_UNORM)
            .name("FrameGraph result image")
            .size(config.width, config.height)
            .tiling(VK_IMAGE_TILING_OPTIMAL)
            .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .build();

        build();
    }

    FrameGraph::FrameGraph(vk::Device* ptr_device, const Window* ptr_window) :
        _ptr_device(ptr_device)
    {
        _surface.emplace(ptr_device, ptr_window);
        build();
    }

    const Size FrameGraph::size() const
    {
        if (_surface)
        {
            const auto& image = &_surface->_images[0];
            return Size
            {
                .width  = image->width,
                .height = image->height
            };
        }

        return Size
        {
            .width  = _image->width,
            .height = _image->height
        };
    }

    void FrameGraph::present() const
    {
        auto command_buffer = _ptr_device->oneTimeSubmitCommandBuffer(
            _ptr_device->queue(),
            "General command buffer"
        );

        command_buffer.write([this](auto command_buffer_handle)
        {
            VkImageMemoryBarrier image_barrier = { };
            image_barrier.sType                             = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            image_barrier.oldLayout                         = VK_IMAGE_LAYOUT_UNDEFINED;
            image_barrier.newLayout                         = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            image_barrier.srcQueueFamilyIndex               = 0;
            image_barrier.dstQueueFamilyIndex               = 0;
            image_barrier.subresourceRange.aspectMask       = VK_IMAGE_ASPECT_COLOR_BIT;
            image_barrier.subresourceRange.baseArrayLayer   = 0;
            image_barrier.subresourceRange.layerCount       = 1;
            image_barrier.subresourceRange.baseMipLevel     = 0;
            image_barrier.subresourceRange.levelCount       = 1;
            image_barrier.image                             = _ptr_output_image->handle;

            vkCmdPipelineBarrier(
                command_buffer_handle,
                VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0, 
                0, nullptr,
                0, nullptr,
                1, &image_barrier 
            );
        }, "Present result image", vk::marker_colors::present_image);

        VkCommandBufferSubmitInfo buffer_submit_info = { };
        buffer_submit_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        buffer_submit_info.commandBuffer    = command_buffer.handle;

        VkSubmitInfo2 submit_info = { };
        submit_info.sType                   = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
        submit_info.pCommandBufferInfos     = &buffer_submit_info;
        submit_info.commandBufferInfoCount  = 1;

        VK_CHECK(
            vkQueueSubmit2(
                _ptr_device->queue().handle,
                1, &submit_info,
                VK_NULL_HANDLE
            )
        );

        VkResult result = VK_SUCCESS;

        VkPresentInfoKHR present_info = { };
        present_info.sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pResults       = &result;
        present_info.pSwapchains    = &_surface->_swapchain_handle;
        present_info.swapchainCount = 1;
        present_info.pImageIndices = &_image_index;

        VK_CHECK(vkQueuePresentKHR(_ptr_device->queue().handle, &present_info));

        VK_CHECK(vkQueueWaitIdle(_ptr_device->queue().handle));
    }
}

namespace pbrlib
{
    void FrameGraph::draw(std::span<const SceneItem*> items)
    {
        if (_ptr_render_pass)
        {
            for (const auto ptr_item: items)
                _ptr_render_pass->render(ptr_item);
        }
        else
        {
            log::engine::error("[FrameGraph] Failed draw scene because render pass is empty");
            return ;
        }

        if (_surface)
            present();

        nextImage();
    }

    void FrameGraph::nextImage()
    {
        if (_surface)
        {
            auto [ptr_image, index] = _surface->nextImage();

            _ptr_output_image   = ptr_image;
            _image_index        = index;
        }
    }
}

namespace pbrlib
{
    void FrameGraph::build()
    {
        createResources();

        _ptr_render_pass = std::make_unique<GBufferGenerator>();

        _ptr_render_pass->addColorOutput("result", _ptr_output_image);
        _ptr_render_pass->depthStencil(&_depth_buffer.value());

        _ptr_render_pass->init(_ptr_device);
    }

    void FrameGraph::createResources()
    {
        const auto [width, height] = size();

        _depth_buffer = vk::Image::Builder(_ptr_device)
            .size(width, height)
            .format(VK_FORMAT_D32_SFLOAT)
            .usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .name("Depth buffer")
            .build();

        nextImage();
    }
}