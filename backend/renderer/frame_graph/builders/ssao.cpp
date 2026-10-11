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

    SSAO& SSAO::srcStage(VkPipelineStageFlags2 src_stage) noexcept
    {
        _src_stage = src_stage;
        return *this;
    }

    SSAO& SSAO::blurImage(vk::Image& ptr_result) noexcept
    {
        _ptr_blur_image = &ptr_result;
        return *this;
    }

    SSAO& SSAO::gbufferDescriptorGroup(vk::DescriptorGroup* gbuffer_descriptor_group) noexcept
    {
        _gbuffer_descriptor_group = gbuffer_descriptor_group;
        return *this;
    }

    void SSAO::validate()
    {
        if (!_ptr_ssao_image) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] image for ssao didn't set");

        if (!_ptr_blur_image) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] image for blur didn't set");

        if (_gbuffer_descriptor_group == nullptr) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] didn't set gbuffer descriptor set");
    
        if (_src_stage == VK_PIPELINE_STAGE_2_NONE)
            throw exception::InvalidState("[ssao::builder] didn't set src stage");
    }

    std::unique_ptr<CompoundRenderPass> SSAO::build()
    {
        validate();

        auto ptr_blur = std::make_unique<BilateralBlur>(_device, *_ptr_blur_image, _blur_settings);
        ptr_blur->apply(*_ptr_ssao_image);

        auto ptr_ssao = std::make_unique<backend::SSAO>(_device, ptr_blur.get());
        ptr_ssao->srcStage(_src_stage);

        ptr_ssao->addColorOutput(AttachmentsTraits<backend::SSAO>::ssao, _ptr_ssao_image);
        ptr_ssao->descriptorGroup(backend::SSAO::gbuffer_set_id, *_gbuffer_descriptor_group);

        auto ptr_compound_render_pass = std::make_unique<CompoundRenderPass>(_device);
        ptr_compound_render_pass->add(std::move(ptr_ssao));
        ptr_compound_render_pass->add(std::move(ptr_blur));

        return ptr_compound_render_pass;
    }
}
