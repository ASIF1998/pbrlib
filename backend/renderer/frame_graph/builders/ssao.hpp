#pragma once

#include <backend/renderer/frame_graph/compound_render_pass.hpp>
#include <backend/renderer/frame_graph/filters/bilateral_blur.hpp>

namespace pbrlib::settings
{
    struct SSAO;
}

namespace pbrlib::backend::builders
{
    class SSAO final
    {
        struct SyncData final
        {
            vk::Image*              ptr_image       = nullptr;
            VkImageLayout           expected_layout = VK_IMAGE_LAYOUT_UNDEFINED;
            VkPipelineStageFlags2   src_stage       = VK_PIPELINE_STAGE_2_NONE;
        };

        void validate();

    public:
        explicit SSAO(vk::Device& device) noexcept;

        SSAO& ssaoImage(vk::Image& image)                       noexcept;
        SSAO& blurImage(vk::Image& image)                       noexcept;
        SSAO& settings(const pbrlib::settings::SSAO& config)    noexcept;

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

        vk::Image* _ptr_ssao_image = nullptr;
        vk::Image* _ptr_blur_image = nullptr;

        std::vector<SyncData> _sync;

        VkDescriptorSet         _gbuffer_set_handle         = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _gbuffer_set_layout_handle  = VK_NULL_HANDLE;

        BilateralBlur::Settings _blur_settings;
    };
}