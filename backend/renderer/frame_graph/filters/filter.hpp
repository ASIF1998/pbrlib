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
        explicit Filter(vk::Device& device, vk::Image& dst_image) noexcept;

        void apply(vk::Image& image);

        [[nodiscard]] vk::Image& srcImage();
        [[nodiscard]] vk::Image& dstImage() noexcept;

    private:
        vk::Image* _ptr_dst_image = nullptr;
        vk::Image* _ptr_src_image = nullptr;
    };
}