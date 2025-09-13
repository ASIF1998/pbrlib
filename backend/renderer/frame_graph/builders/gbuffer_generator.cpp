#include <backend/renderer/frame_graph/builders/gbuffer_generator.hpp>
#include <backend/renderer/frame_graph/gbuffer_generator.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::builders
{
    GBufferGenerator::GBufferGenerator(vk::Device& device) noexcept :
        _device (device)
    { }

    GBufferGenerator& GBufferGenerator::posUvImage(vk::Image& image) noexcept
    {
        _ptr_pos_uv_image = &image;
        return *this;
    }
    
    GBufferGenerator& GBufferGenerator::normalTangentImage(vk::Image& image) noexcept
    {
        _ptr_nor_tan_image = &image;
        return *this;
    }
    
    GBufferGenerator& GBufferGenerator::materialIndexImage(vk::Image& image) noexcept
    {
        _ptr_mat_index_image = &image;
        return *this;
    }
    
    GBufferGenerator& GBufferGenerator::depthStencilImage(vk::Image& image) noexcept
    {
        _ptr_depth_stencil_image = &image;
        return *this;
    }

    void GBufferGenerator::validate()
    {
        if (!_ptr_pos_uv_image) [[unlikely]]
            throw exception::InvalidState("[gbuffer-generator::builder] image for positions and uvs didn't set");
        
        if (!_ptr_nor_tan_image) [[unlikely]]
            throw exception::InvalidState("[gbuffer-generator::builder] image for normals and tangents didn't set");
        
        if (!_ptr_mat_index_image) [[unlikely]]
            throw exception::InvalidState("[gbuffer-generator::builder] image for materials indices didn't set");
        
        if (!_ptr_depth_stencil_image) [[unlikely]]
            throw exception::InvalidState("[gbuffer-generator::builder] image for depth-stencil didn't set");
    }

    std::unique_ptr<RenderPass> GBufferGenerator::build()
    {
        validate();

        std::unique_ptr<RenderPass> ptr_gbuffer_generator = std::make_unique<backend::GBufferGenerator>(_device);

        constexpr auto pos_uv           = AttachmentsTraits<backend::GBufferGenerator>::pos_uv;
        constexpr auto normal_tangent   = AttachmentsTraits<backend::GBufferGenerator>::normal_tangent;
        constexpr auto material_index   = AttachmentsTraits<backend::GBufferGenerator>::material_index;

        ptr_gbuffer_generator->addColorOutput(pos_uv, _ptr_pos_uv_image);
        ptr_gbuffer_generator->addColorOutput(normal_tangent, _ptr_nor_tan_image);
        ptr_gbuffer_generator->addColorOutput(material_index, _ptr_mat_index_image);
        ptr_gbuffer_generator->depthStencil(_ptr_depth_stencil_image);

        ptr_gbuffer_generator->addSyncImage (
            _ptr_depth_stencil_image, 
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT , ptr_gbuffer_generator->srcStage()
        );

        return ptr_gbuffer_generator;
    }
}