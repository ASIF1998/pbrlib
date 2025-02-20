#pragma once

#include <backend/renderer/vulkan/image.hpp>

#include <vulkan/vulkan.h>

#include <vector>
#include <numeric>

namespace pbrlib
{
    class Window;
    class FrameGraph;
}

namespace pbrlib::vk
{
    struct NextImageInfo
    {
        const vk::Image*    ptr_image   = nullptr;
        uint32_t            index       = std::numeric_limits<uint32_t>::max();
    };

    class Surface
    {
        friend class FrameGraph;

        std::vector<VkSurfaceFormatKHR> getSurfaceFormats();

        void createSurface(const Window* ptr_window);
        void createSwapchain(const Window* ptr_window);
        void getImages(uint32_t width, uint32_t height);
        void createImageViews(uint32_t width, uint32_t height);

    public:
        explicit Surface(const Device* ptr_device, const Window* ptr_window);

        Surface(Surface&& surface);
        Surface(const Surface& surface) = delete;

        ~Surface();

        Surface& operator = (Surface&& surface);
        Surface& operator = (const Surface& surface) = delete;

        [[nodiscard]] NextImageInfo nextImage() const;
        
    private:
        VkSurfaceKHR    _surface_handle     = VK_NULL_HANDLE;
        VkSwapchainKHR  _swapchain_handle   = VK_NULL_HANDLE;

        std::vector<vk::Image> _images;

        VkSurfaceFormatKHR _surface_format = { };

        const Device* _ptr_device = nullptr;

        mutable uint32_t _current_image_index = 0; 
    };
}