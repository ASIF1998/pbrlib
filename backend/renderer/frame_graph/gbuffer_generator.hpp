#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

#include <optional>
#include <array>

namespace pbrlib::backend
{
    class GBufferGenerator;

    template<>
    struct AttachmentsTraits<GBufferGenerator>
    {
        static constexpr auto metadata()
        {
            constexpr auto usage_flags = 
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT 
            |   VK_IMAGE_USAGE_SAMPLED_BIT 
            |   VK_IMAGE_USAGE_TRANSFER_SRC_BIT
            |   VK_IMAGE_USAGE_TRANSFER_DST_BIT
            |   VK_IMAGE_USAGE_STORAGE_BIT;

            constexpr std::array metadata
            {
                AttachmentMetadata(pos_uv, VK_FORMAT_R32G32B32A32_SFLOAT, usage_flags),
                AttachmentMetadata(normal_tangent, VK_FORMAT_R32G32B32A32_SFLOAT, usage_flags),
                AttachmentMetadata(material_index, VK_FORMAT_R16_UINT, usage_flags)
            };

            return metadata;
        }

        constexpr static auto pos_uv            = "gbuffer-pos-uv";
        constexpr static auto normal_tangent    = "gbuffer-normal-tangent";
        constexpr static auto material_index    = "gbuffer-material-index";
    };
}

namespace pbrlib::backend
{
    struct GBufferPushConstantBlock final
    {
        math::mat4  projection_view;
        uint32_t    instance_id     = ~0u;
        uint32_t    material_index  = ~0u;
    };

    class GBufferGenerator final :
        public RenderPass
    {
        void createResultDescriptorSet();

        bool init(const RenderContext& context, uint32_t width, uint32_t height) override;
        
        bool createPipeline();
        
        void beginPass(vk::CommandBuffer& command_buffer);
        void render(vk::CommandBuffer& command_buffer) override;
        void endPass(vk::CommandBuffer& command_buffer);
        
        void createRenderPass();
        void createFramebuffer();

        void createSampler();
        void initResultDescriptorSet();

        void setupColorAttachmentsLayout();

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

    public:
        explicit GBufferGenerator(vk::Device& device);

    private:
        vk::FramebufferHandle _framebuffer_handle;

        vk::PipelineLayoutHandle            _pipeline_layout_handle;
        vk::RenderPassHandle                _render_pass_handle;
        vk::PipelineHandle                  _pipeline_handle;

        GBufferPushConstantBlock _push_constant_block;

        vk::DescriptorSetLayoutHandle   _result_descriptor_set_layout_handle;
        vk::DescriptorSetHandle         _result_descriptor_set_handle;

        vk::SamplerHandle _sampler_handle;

        static constexpr auto _final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    };
}