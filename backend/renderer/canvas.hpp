#pragma once

#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/surface.hpp>

#include <pbrlib/event_system.hpp>

namespace pbrlib
{
    struct Config;
}

namespace pbrlib::backend
{
    struct Size final
    {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    class Canvas final :
        public pbrlib::EventSystem
    {
        [[nodiscard]] bool nextImage(VkSemaphore image_available_semaphore);

    public:
        explicit Canvas(vk::Device& device, const pbrlib::Window* ptr_window);
        explicit Canvas(vk::Device& device, uint32_t width, uint32_t height);

        Canvas(Canvas&& canvas)         = delete;
        Canvas(const Canvas& canvas)    = delete;

        Canvas& operator = (Canvas&& canvas)        = delete;
        Canvas& operator = (const Canvas& canvas)   = delete;

        void present(const vk::Image* ptr_result, VkSemaphore image_available_semaphore, VkSemaphore render_finished_semaphores);

        [[nodiscard]] Size      size()              const;
        [[nodiscard]] uint8_t   framesInFlight()    const noexcept;

    private:
        vk::Device& _device;

        std::optional<vk::Image> _image;

        struct
        {
            std::optional<vk::Surface>  vk_surface;
            vk::Image*                  ptr_image   = nullptr;
            uint32_t                    index       = 0;
        } _surface;
    };
}
