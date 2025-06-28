#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

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
        bool rebuild(vk::Device& device, const RenderContext& context)  override;

        void render(size_t item_id, vk::CommandBuffer& command_buffer)  override;
        void prePass(vk::CommandBuffer& command_buffer)                 override;
        void postPass(vk::CommandBuffer& command_buffer)                override;
    };
}