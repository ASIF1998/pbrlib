#pragma once

#include <backend/renderer/vulkan/image.hpp>

#include <vector>
#include <numeric>
#include <optional>

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

        void createSurface();
        void createSwapchain();
        void getImages(uint32_t width, uint32_t height);
        void createImageViews(uint32_t width, uint32_t height);

        void create();

    public:
        explicit Surface(Device& device, const pbrlib::Window& window);

        Surface(Surface&& surface);

        [[nodiscard]] std::optional<NextImageInfo> nextImage();

    private:
        const pbrlib::Window& _window;

        SurfaceHandle       _surface_handle;
        SwapchainHandle     _swapchain_handle;

        std::vector<vk::Image> _images;

        VkSurfaceFormatKHR _surface_format = { };

        Device& _device;

        mutable uint32_t _current_image_index = 0;

        FenceHandle _next_image_fence_handle;
    };
}
