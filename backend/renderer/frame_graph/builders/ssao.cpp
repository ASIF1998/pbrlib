#include <backend/renderer/frame_graph/ssao.hpp>
#include <backend/renderer/frame_graph/builders/ssao.hpp>
#include <backend/renderer/frame_graph/filters/bilateral_blur.hpp>

#include <pbrlib/config.hpp>
#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::builders
{
    SSAO::SSAO(vk::Device& device) noexcept :
        _device (device)
    { }

    SSAO& SSAO::ssaoImage(vk::Image& ptr_result) noexcept
    {
        _ptr_ssao_image = &ptr_result;
        return *this;
    }

    SSAO& SSAO::settings(const pbrlib::settings::SSAO& config) noexcept
    {
        _blur_settings = 
        {
            .sample_count   = config.blur_samples_count,
            .sigma_s        = config.spatial_sigma,
            .sigma_l        = config.luminance_sigma
        };

        return *this;
    }
    
    SSAO& SSAO::blurImage(vk::Image& ptr_result) noexcept
    {
        _ptr_blur_image = &ptr_result;
        return *this;
    }

    SSAO& SSAO::addSync (
        vk::Image*              ptr_image, 
        VkImageLayout           expected_layout, 
        VkPipelineStageFlags2   src_stage
    ) 
    {
        _sync.emplace_back(ptr_image, expected_layout, src_stage);
        return *this;
    }

    SSAO& SSAO::gbufferDescriptorSet(VkDescriptorSet set_handle, VkDescriptorSetLayout set_layout_handle) noexcept
    {
        _gbuffer_set_handle         = set_handle;
        _gbuffer_set_layout_handle  = set_layout_handle;
        return *this;
    }

    void SSAO::validate()
    {
        if (!_ptr_ssao_image) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] image for ssao didn't set");
        
        if (!_ptr_blur_image) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] image for blur didn't set");

        if (_sync.empty()) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] didn't set gbuffer sync data");

        if (_gbuffer_set_handle == VK_NULL_HANDLE || _gbuffer_set_layout_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] didn't set gbuffer descriptor set");
    }

    std::unique_ptr<CompoundRenderPass> SSAO::build()
    {
        validate();

        auto ptr_compound_render_pass = std::make_unique<CompoundRenderPass>(_device);

        std::unique_ptr<BilateralBlur> ptr_blur = std::make_unique<BilateralBlur>(_device, AttachmentsTraits<backend::SSAO>::blur, _blur_settings);

        ptr_blur->addSyncImage (
            _ptr_ssao_image, 
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
            VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
        );
        
        ptr_blur->addSyncImage (
            _ptr_blur_image, 
            VK_IMAGE_LAYOUT_GENERAL, 
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
        );

        std::unique_ptr<RenderPass> ptr_ssao = std::make_unique<backend::SSAO>(_device, ptr_blur.get());

        ptr_ssao->addSyncImage (
            _ptr_ssao_image, 
            VK_IMAGE_LAYOUT_GENERAL, 
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
        );

        ptr_ssao->addColorOutput(AttachmentsTraits<backend::SSAO>::ssao, _ptr_ssao_image);

        for (auto& sync_data: _sync)
        {
            const auto dst_stage = sync_data.expected_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL ? ptr_ssao->srcStage() : ptr_ssao->dstStage();

            ptr_ssao->addSyncImage (
                sync_data.ptr_image,
                sync_data.expected_layout,
                sync_data.src_stage, dst_stage
            );
        }

        ptr_ssao->descriptorSet(InputDescriptorSetTraits<backend::SSAO>::gbuffer, _gbuffer_set_handle, _gbuffer_set_layout_handle);

        ptr_blur->addColorOutput(AttachmentsTraits<backend::SSAO>::blur, _ptr_blur_image);
        ptr_blur->apply(*_ptr_ssao_image);
        
        ptr_compound_render_pass->add(std::move(ptr_ssao));
        ptr_compound_render_pass->add(std::move(ptr_blur));

        return ptr_compound_render_pass;
    }
}