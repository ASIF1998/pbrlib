#include <backend/renderer/frame_graph/ssao.hpp>
#include <backend/renderer/frame_graph/builders/ssao.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::builders
{
    SSAO::SSAO(vk::Device& device) noexcept :
        _device (device)
    { }

    SSAO& SSAO::resultImage(vk::Image* ptr_result) noexcept
    {
        _ptr_result = ptr_result;
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
        if (!_ptr_result) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] result image didn't set");

        if (_sync.empty()) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] didn't set gbuffer sync data");

        if (_gbuffer_set_handle == VK_NULL_HANDLE || _gbuffer_set_layout_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidState("[ssao::builder] didn't set gbuffer descriptor set");
    }

    std::unique_ptr<CompoundRenderPass> SSAO::build()
    {
        validate();

        auto ptr_ssao2 = std::make_unique<CompoundRenderPass>(_device);

        std::unique_ptr<RenderPass> ptr_ssao = std::make_unique<backend::SSAO>(_device);

        ptr_ssao->addSyncImage (
            _ptr_result, 
            VK_IMAGE_LAYOUT_GENERAL, 
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
        );

        ptr_ssao->addColorOutput(SSAOOutputAttachmentsNames::result, _ptr_result);

        
        for (auto& sync_data: _sync)
        {
            const auto dst_stage = sync_data.expected_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL ? ptr_ssao->srcStage() : ptr_ssao->dstStage();

            ptr_ssao->addSyncImage (
                sync_data.ptr_image,
                sync_data.expected_layout,
                sync_data.src_stage, dst_stage
            );
        }

        ptr_ssao->descriptorSet(SSAOInputSetsId::eGBuffer, _gbuffer_set_handle, _gbuffer_set_layout_handle);

        ptr_ssao2->add(std::move(ptr_ssao));

        return ptr_ssao2;
    }
}