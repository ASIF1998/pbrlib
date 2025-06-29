#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

namespace pbrlib::backend::vk
{
    class Device;
    class Image;
}

namespace pbrlib::backend::vk::builders
{
    class RenderPass final
    {
    public:
        explicit RenderPass(Device& device) noexcept;

        RenderPass(RenderPass&& builder)        = delete;
        RenderPass(const RenderPass& builder)   = delete;

        RenderPass& operator = (RenderPass&& builder)       = delete;
        RenderPass& operator = (const RenderPass& builder)  = delete;

        RenderPass& addColorAttachment (
            const vk::Image*    ptr_image, 
            VkImageLayout       final_layout
        );

        RenderPass& depthAttachment (
            const vk::Image*    ptr_image, 
            VkImageLayout       final_layout
        );

        [[nodiscard]] VkRenderPass build();
    
    private:
        Device& _device;

        std::vector<VkAttachmentDescription2>   _attachments;
        std::vector<VkAttachmentReference2>     _color_attachments_refs;
        std::optional<VkAttachmentReference2>   _depth_attachment_ref;
    };
}