#include <backend/renderer/frame_graph/filters/filter.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend
{
    Filter::Filter(vk::Device& device, vk::Image& dst_image) noexcept :
        RenderPass      (device),
        _ptr_dst_image  (&dst_image)
    { }

    void Filter::apply(vk::Image& image)
    {
        _ptr_src_image = &image;

        constexpr auto dst_stage = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;

        addSyncImage (
            _ptr_src_image,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT, dst_stage
        );

        addSyncImage (
            _ptr_dst_image,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, dst_stage
        );
    }

    vk::Image& Filter::srcImage()
    {
        if (!_ptr_src_image) [[unlikely]]
            throw exception::InvalidState("[filter] didn't set source image");
        
        return *_ptr_src_image;
    }

    vk::Image& Filter::dstImage() noexcept
    {
        return *_ptr_dst_image;
    }
}
