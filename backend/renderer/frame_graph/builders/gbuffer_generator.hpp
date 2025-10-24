#pragma once

#include <memory>

namespace pbrlib::backend
{
    class RenderPass;
}

namespace pbrlib::backend::vk
{
    class Device;
    class Image;
}

namespace pbrlib::backend::builders
{
    class GBufferGenerator final
    {
        void validate();
        
    public:
        explicit GBufferGenerator(vk::Device& device) noexcept;

        GBufferGenerator& posUvImage(vk::Image& image)          noexcept;
        GBufferGenerator& normalTangentImage(vk::Image& image)  noexcept;
        GBufferGenerator& materialIndexImage(vk::Image& image)  noexcept;
        GBufferGenerator& depthStencilImage(vk::Image& image)   noexcept;

        [[nodiscard]]
        std::unique_ptr<RenderPass> build();

    private:
        vk::Device& _device;

        vk::Image* _ptr_pos_uv_image        = nullptr;
        vk::Image* _ptr_nor_tan_image       = nullptr;
        vk::Image* _ptr_mat_index_image     = nullptr;
        vk::Image* _ptr_depth_stencil_image = nullptr;
    };
}