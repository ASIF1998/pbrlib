#include <backend/renderer/vulkan/render_pass.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <backend/utils/vulkan.hpp>

namespace pbrlib::backend::vk::builders
{
    RenderPass::RenderPass(Device& device) noexcept :
        _device (device)
    { }

    RenderPass& RenderPass::addColorAttachment (
        const vk::Image*    ptr_image, 
        VkImageLayout       final_layout
    )
    {
        _attachments.emplace_back
        (
            VkAttachmentDescription2
            {
                .sType          = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2,
                .format         = ptr_image->format,
                .samples        = VK_SAMPLE_COUNT_1_BIT,
                .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
                .initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .finalLayout    = final_layout
            }
        );

        _color_attachments_refs.emplace_back
        (
            VkAttachmentReference2
            {
                .sType      = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                .attachment = static_cast<uint32_t>(_attachments.size() - 1),
                .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
            }
        );

        return *this;
    }

    RenderPass& RenderPass::depthAttachment (
        const vk::Image*    ptr_image, 
        VkImageLayout       final_layout
    )
    {
        _attachments.emplace_back
        (
            VkAttachmentDescription2
            {
                .sType          = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2,
                .format         = ptr_image->format,
                .samples        = VK_SAMPLE_COUNT_1_BIT,
                .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
                .initialLayout  = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
                .finalLayout    = final_layout
            }
        );

        _depth_attachment_ref = VkAttachmentReference2
        {
            .sType      = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
            .attachment = static_cast<uint32_t>(_attachments.size() - 1),
            .layout     = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT
        };

        return *this;
    }

    RenderPassHandle RenderPass::build()
    {
        if (_color_attachments_refs.empty())
            throw exception::InvalidState("[render-pass-builder] color attachment count is 0");

        VkSubpassDescription2 subpass_description
        {
            .sType                      = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2,
            .pipelineBindPoint          = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount       = static_cast<uint32_t>(_color_attachments_refs.size()),
            .pColorAttachments          = _color_attachments_refs.data()
        };

        if (_depth_attachment_ref)
            subpass_description.pDepthStencilAttachment = &_depth_attachment_ref.value();

        const VkRenderPassCreateInfo2 render_pass_create_info
        {
            .sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2,
            .attachmentCount    = static_cast<uint32_t>(_attachments.size()),
            .pAttachments       = _attachments.data(),
            .subpassCount       = 1,
            .pSubpasses         = &subpass_description
        };

        VkRenderPass render_pass_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateRenderPass2(
            _device.device(),
            &render_pass_create_info,
            nullptr,
            &render_pass_handle
        ));

        return RenderPassHandle(render_pass_handle);
    }
}