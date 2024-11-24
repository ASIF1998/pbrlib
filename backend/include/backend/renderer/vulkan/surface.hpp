#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib
{
    class Window;
}

namespace pbrlib::vk
{
    class Surface
    {
        std::vector<VkSurfaceFormatKHR> getSurfaceFormats();

        void createSurface(const Window* ptr_window);
        void createSwapchain(const Window* ptr_window);
        void getImages();
        void createImageViews(uint32_t width, uint32_t height);

    public:
        explicit Surface(const Device* ptr_device, const Window* ptr_window);

        Surface(Surface&& surface);
        Surface(const Surface& surface) = delete;

        ~Surface();

        Surface& operator = (Surface&& surface);
        Surface& operator = (const Surface& surface) = delete;
        
    private:
        VkSurfaceKHR    _surface_handle     = VK_NULL_HANDLE;
        VkSwapchainKHR  _swapchain_handle   = VK_NULL_HANDLE;

        std::vector<VkImage>        _image_handles;
        std::vector<VkImageView>    _image_view_handles;

        VkSurfaceFormatKHR _surface_format = { };

        const Device* _ptr_device = nullptr;
    };
}