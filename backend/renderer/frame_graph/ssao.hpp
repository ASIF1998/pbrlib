#pragma once

#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <backend/renderer/frame_graph/render_pass.hpp> 

#include <optional>

namespace pbrlib::backend
{
    struct SSAOOutputAttachmentsNames final
    {
        constexpr static auto result = "ssao-result";
    };

    struct SSAOInputAttachmentNames final
    {
        constexpr static auto pos_uv            = "ssao-gbuffer-pos-uv";
        constexpr static auto normal_tangent    = "ssao-gbuffer-normal-tangent";
    };

    struct SSAOInputSetsId
    {
        enum
        {
            eGBuffer
        };
    };
    
    class SSAO final :
        public RenderPass
    {
        bool init(const RenderContext& context, uint32_t width, uint32_t height)    override;
        bool rebuild()                                                              override;

        void render(vk::CommandBuffer& command_buffer) override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

        void createSampler();
        void createResultDescriptorSet();

        void createSSAODescriptorSet();

    public:
        explicit SSAO(vk::Device& device);
        ~SSAO();

    private:
        std::optional<vk::PipelineLayout>   _pipeline_layout;

        VkPipeline _pipeline_handle = VK_NULL_HANDLE;

        VkDescriptorSet         _result_image_desc_set          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _result_image_desc_set_layout   = VK_NULL_HANDLE;

        VkSampler _result_image_sampler = VK_NULL_HANDLE;

        VkDescriptorSet         _ssao_desc_set          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _ssao_desc_set_layout   = VK_NULL_HANDLE;

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    };
}