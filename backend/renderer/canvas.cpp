#include <backend/renderer/canvas.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/utils/vulkan.hpp>

#include <backend/profiling.hpp>

#include <backend/utils/align_size.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend
{
    Canvas::Canvas(vk::Device& device, const pbrlib::Window* ptr_window) :
        _device(device)
    {
        _surface.vk_surface.emplace(_device, ptr_window);
    }

    Canvas::Canvas(vk::Device& device, uint32_t width, uint32_t height) :
        _device(device)
    {
        const uint32_t groupSize = device.workGroupSize();

        width   = backend::utils::alignSize(width, groupSize);
        height  = backend::utils::alignSize(height, groupSize);

        _image = vk::builders::Image(_device)
            .addQueueFamilyIndex(_device.queue().family_index)
            .fillColor(math::vec3(0))
            .format(VK_FORMAT_R8G8B8A8_UNORM)
            .name("result-image")
            .size(width, height)
            .tiling(VK_IMAGE_TILING_OPTIMAL)
            .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .build();
    }

    void Canvas::nextImage()
    {
        if (_surface.vk_surface) [[likely]]
        {
            auto [ptr_image, index] = _surface.vk_surface->nextImage();

            _surface.index      = index;
            _surface.ptr_image  = ptr_image;
        }
    }

    void Canvas::present(const vk::Image* ptr_result)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!_surface.vk_surface) [[unlikely]]
            return ;

        nextImage();

        _surface.ptr_image->changeLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("present");

        command_buffer.write([this, ptr_result] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "present-result-upload");

            const auto width = static_cast<int32_t>(ptr_result->width);
            const auto height  = static_cast<int32_t>(ptr_result->height);

            const VkImageBlit image_blit 
            {
                .srcSubresource = 
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel       = 0,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                },
                .srcOffsets = {{0}, {width, height, 1}},
                .dstSubresource =
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel       = 0,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                },
                .dstOffsets = {{0}, {width, height, 1}},
            };

            vkCmdBlitImage(
                command_buffer_handle, 
                ptr_result->handle.handle(),
                ptr_result->layout,
                _surface.ptr_image->handle.handle(),
                _surface.ptr_image->layout,
                1, &image_blit, 
                VK_FILTER_NEAREST
            );
        }, "present-result-upload", vk::marker_colors::write_data_in_image);

        _device.submit(command_buffer);

        _surface.ptr_image->changeLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        VkResult result = VK_SUCCESS;

        const VkPresentInfoKHR present_info 
        { 
            .sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .swapchainCount = 1,
            .pSwapchains    = &_surface.vk_surface->_swapchain_handle.handle(),
            .pImageIndices  = &_surface.index,
            .pResults       = &result
        };

        VK_CHECK(vkQueuePresentKHR(_device.queue().handle, &present_info));
    }

    Size Canvas::size() const
    {
        const auto ptr_image = _surface.vk_surface
            ?   &_surface.vk_surface->_images[0]
            :   &_image.value();

        return Size
        {
            .width  = ptr_image->width,
            .height = ptr_image->height
        };
    }
}