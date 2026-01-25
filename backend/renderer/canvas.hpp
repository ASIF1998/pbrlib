#pragma once

#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/surface.hpp>

#include <optional>

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

    class Canvas final
    {
        void nextImage();

    public:
        explicit Canvas(vk::Device& device, const pbrlib::Window* ptr_window);
        explicit Canvas(vk::Device& device, uint32_t width, uint32_t height);

        Canvas(Canvas&& canvas)         = default;
        Canvas(const Canvas& canvas)    = delete;

        Canvas& operator = (const Canvas& canvas) = delete;

        void present(const vk::Image* ptr_result);

        [[nodiscard]]
        Size size() const;

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
