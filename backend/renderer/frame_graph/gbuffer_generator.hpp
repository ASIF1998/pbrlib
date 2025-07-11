#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <optional>

namespace pbrlib::backend
{
    struct GBufferAttachmentsName final
    {
        constexpr static auto pos_uv            = "gbuffer-pos-uv";
        constexpr static auto normal_tangent    = "gbuffer-normal-tangent";
        constexpr static auto material_index    = "gbuffer-material-index";
    };

    struct GBufferDescriptorSetBindings
    {
        enum 
        {
            ePosUv,
            eNormalTangent,
            eMaterialIndices
        };
    };

    struct GBufferPushConstantBlock final
    {
        math::mat4  projection_view;
        uint32_t    instance_id;
        uint32_t    material_index;
    };

    class GBufferGenerator final :
        public RenderPass
    {
        void createResultDescriptorSet();

        bool init(const RenderContext& context, uint32_t width, uint32_t height)    override;
        bool rebuild()                                                              override;
        
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
        ~GBufferGenerator();

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    private:
        VkPipeline                          _pipeline_handle        = VK_NULL_HANDLE;
        VkRenderPass                        _render_pass_handle     = VK_NULL_HANDLE;
        VkPipelineLayout                    _pipeline_layout_handle = VK_NULL_HANDLE;

        VkFramebuffer _framebuffer_handle = VK_NULL_HANDLE;

        GBufferPushConstantBlock _push_constant_block;

        VkDescriptorSet         _result_descriptor_set_handle           = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _result_descriptor_set_layout_handle    = VK_NULL_HANDLE;

        VkSampler _sampler_handle = VK_NULL_HANDLE;
    };
}