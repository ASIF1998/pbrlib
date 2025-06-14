#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

namespace pbrlib::backend::vk
{
    class Device;
    class Image;
}

namespace pbrlib::backend::vk
{
    class RenderPassBuilder final
    {
    public:
        explicit RenderPassBuilder(Device& device);

        RenderPassBuilder(RenderPassBuilder&& builder)      = delete;
        RenderPassBuilder(const RenderPassBuilder& builder) = delete;

        RenderPassBuilder& operator = (RenderPassBuilder&& builder)         = delete;
        RenderPassBuilder& operator = (const RenderPassBuilder& builder)    = delete;

        RenderPassBuilder& addColorAttachment (
            const Image*    ptr_image, 
            VkImageLayout   final_layout
        );

        RenderPassBuilder& depthAttachment (
            const Image*    ptr_image, 
            VkImageLayout   final_layout
        );

        [[nodiscard]] VkRenderPass build();
    
    private:
        Device& _device;

        std::vector<VkAttachmentDescription2>   _attachments;
        std::vector<VkAttachmentReference2>     _color_attachments_refs;
        std::optional<VkAttachmentReference2>   _depth_attachment_ref;
    };
}