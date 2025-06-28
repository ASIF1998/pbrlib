#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <optional>

namespace pbrlib::backend
{
    struct GBufferFinalAttachmentsName final
    {
        constexpr static auto pos_uv            = "gbuffer-pos-uv";
        constexpr static auto normal_tangent    = "gbuffer-normal-tangent";
        constexpr static auto material_index    = "gbuffer-material-index";
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
        bool init(vk::Device& device, const RenderContext& context)     override;
        bool rebuild(vk::Device& device, const RenderContext& context)  override;

        void render(size_t item_id, vk::CommandBuffer& command_buffer)  override;
        void prePass(vk::CommandBuffer& command_buffer)                 override;
        void postPass(vk::CommandBuffer& command_buffer)                override;
        
        void createRenderPass();
        void createFramebuffer();

        void setupColorAttachmentsLayout();

    public:
        ~GBufferGenerator();

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    private:
        uint32_t _width     = 0;
        uint32_t _height    = 0;

        VkPipeline                          _pipeline_handle    = VK_NULL_HANDLE;
        VkRenderPass                        _render_pass_handle = VK_NULL_HANDLE;
        std::optional<vk::PipelineLayout>   _pipeline_layout;

        VkFramebuffer _framebuffer_handle = VK_NULL_HANDLE;

        GBufferPushConstantBlock _push_constant_block;
    };
}