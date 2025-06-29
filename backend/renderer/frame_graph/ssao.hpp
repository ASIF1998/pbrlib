#pragma once

#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <backend/renderer/frame_graph/render_pass.hpp> 

#include <optional>

namespace pbrlib::backend
{
    struct SSAOAttachmentsName final
    {
        constexpr static auto result = "ssao-result";
    };
    
    class SSAO final :
        public RenderPass
    {
        bool init(vk::Device& device, const RenderContext& context)     override;
        bool rebuild()                                                  override;

        void render(size_t item_id, vk::CommandBuffer& command_buffer)  override;
        void prePass(vk::CommandBuffer& command_buffer)                 override;
        void postPass(vk::CommandBuffer& command_buffer)                override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

    public:
        ~SSAO();

    private:
        std::optional<vk::PipelineLayout>   _pipeline_layout;
        VkPipeline                          _pipeline_handle = VK_NULL_HANDLE;

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    };
}