#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

namespace pbrlib::backend::vk
{
    class Image;
}

namespace pbrlib::backend
{
    class Filter :
        public RenderPass
    {
    public:
        explicit Filter(vk::Device& device) noexcept;

        void apply(vk::Image& image) noexcept;

        [[nodiscard]] vk::Image& srcImage();

    private:
        vk::Image* _ptr_src_image = nullptr;
    };
}