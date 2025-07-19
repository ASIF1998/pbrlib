#pragma once

#include <backend/renderer/frame_graph/compound_render_pass.hpp>

namespace pbrlib::backend::builders
{
    class SSAO final
    {
        struct SyncData
        {
            vk::Image*              ptr_image       = nullptr;
            VkImageLayout           expected_layout = VK_IMAGE_LAYOUT_UNDEFINED;
            VkPipelineStageFlags2   src_stage       = VK_PIPELINE_STAGE_2_NONE;
        };

        void validate();

    public:
        explicit SSAO(vk::Device& device) noexcept;

        SSAO& resultImage(vk::Image* ptr_result) noexcept;

        SSAO& addSync (
            vk::Image*              ptr_image, 
            VkImageLayout           expected_layout, 
            VkPipelineStageFlags2   src_stage
        );

        SSAO& gbufferDescriptorSet(VkDescriptorSet set_handle, VkDescriptorSetLayout set_layout_handle) noexcept;

        [[nodiscard]]
        std::unique_ptr<CompoundRenderPass> build();

    private:
        vk::Device& _device;

        vk::Image* _ptr_result;

        std::vector<SyncData> _sync;

        VkDescriptorSet         _gbuffer_set_handle         = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _gbuffer_set_layout_handle  = VK_NULL_HANDLE;
    };
}