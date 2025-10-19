#pragma once

#include <backend/renderer/vulkan/image.hpp>

#include <vulkan/vulkan.h>

#include <vector>
#include <numeric>

namespace pbrlib
{
    class Window;
}

namespace pbrlib::backend
{
    class Canvas;
}

namespace pbrlib::backend::vk
{
    struct NextImageInfo final
    {
        vk::Image*  ptr_image   = nullptr;
        uint32_t    index       = std::numeric_limits<uint32_t>::max();
    };

    class Surface final
    {
        friend class pbrlib::backend::Canvas;

        [[nodiscard]]
        std::vector<VkSurfaceFormatKHR> getSurfaceFormats();

        void createSurface(const pbrlib::Window* ptr_window);
        void createSwapchain(const pbrlib::Window* ptr_window);
        void getImages(uint32_t width, uint32_t height);
        void createImageViews(uint32_t width, uint32_t height);

    public:
        explicit Surface(Device& device, const pbrlib::Window* ptr_window);

        Surface(Surface&& surface);
        Surface(const Surface& surface) = delete;

        ~Surface();

        Surface& operator = (Surface&& surface);
        Surface& operator = (const Surface& surface) = delete;

        [[nodiscard]] NextImageInfo nextImage();
        
    private:
        SurfaceHandle       _surface_handle;
        SwapchainHandle     _swapchain_handle;

        std::vector<vk::Image> _images;

        VkSurfaceFormatKHR _surface_format = { };

        Device& _device;

        mutable uint32_t _current_image_index = 0; 

        VkFence _next_image_fence_handle = VK_NULL_HANDLE;
    };
}