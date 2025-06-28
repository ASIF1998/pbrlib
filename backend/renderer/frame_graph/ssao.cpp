#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/render_pass.hpp>
#include <backend/renderer/vulkan/framebuffer.hpp>

namespace pbrlib::backend
{
    SSAO::~SSAO()
    {
        const auto device_handle = _ptr_device->device();

        vkDestroyRenderPass(device_handle, _render_pass_handle, nullptr);
        vkDestroyFramebuffer(device_handle, _framebuffer_handle, nullptr);
    }

    bool SSAO::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(device, context))
        {
            log::error("[ssao] failed initialize");
            return false;
        }

        /// @todo вроде как render pass должен быть один на несколько подпроходов

        const auto* ptr_result_attachment = colorOutputAttach(SSAOAttachmentsName::result);

        _render_pass_handle = vk::RenderPassBuilder(*_ptr_device)
            .addColorAttachment(ptr_result_attachment, final_attachments_layout)
            .build();

        _pipeline_layout = vk::PipelineLayout::Builder(*_ptr_device)
            .build();

        _framebuffer_handle = vk::FramebufferBuild(*_ptr_device)
            .size(ptr_result_attachment->width, ptr_result_attachment->height)
            .layers(1)
            .renderPass(_render_pass_handle)
            .addAttachment(*ptr_result_attachment)
            .build();

        return rebuild(*_ptr_device, *_ptr_context);;
    }

    bool SSAO::rebuild(vk::Device& device, const RenderContext& context)
    {
        return true;
    }

    void SSAO::render(size_t item_id, vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    void SSAO::prePass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    void SSAO::postPass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }
}