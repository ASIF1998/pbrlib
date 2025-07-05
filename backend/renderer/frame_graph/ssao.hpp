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
    
    class SSAO final :
        public RenderPass
    {
        bool init(vk::Device& device, const RenderContext& context)     override;
        bool rebuild()                                                  override;

        void render(vk::CommandBuffer& command_buffer) override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

    public:
        ~SSAO();

    private:
        std::optional<vk::PipelineLayout>   _pipeline_layout;
        VkDescriptorSet                     _descriptor_set_handle  = VK_NULL_HANDLE;
        VkPipeline                          _pipeline_handle        = VK_NULL_HANDLE;

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    };
}