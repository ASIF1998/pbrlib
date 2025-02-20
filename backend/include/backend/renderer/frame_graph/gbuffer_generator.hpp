#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib
{
    class GBufferGenerator :
        public RenderPass
    {
        bool init(const vk::Device* ptr_device) override;
        void render(const SceneItem* ptr_item)  override;
        
        void createPipeline();
        void createPipelineLayout();
        void createRenderPass();
        void createFramebuffer();

    public:
        ~GBufferGenerator();

    private:
        const vk::Device* _ptr_device = nullptr;

        VkPipeline          _pipeline_handle        = VK_NULL_HANDLE;
        VkPipelineLayout    _pipeline_layout_handle = VK_NULL_HANDLE;
        VkRenderPass        _render_pass_handle     = VK_NULL_HANDLE;

        VkFramebuffer _framebuffer_handle = VK_NULL_HANDLE;
    };
}