#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

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
            const Image*    ptr_image,
            VkImageLayout   final_layout
        );

        RenderPass& depthAttachment (
            const Image*    ptr_image,
            VkImageLayout   final_layout
        );

        [[nodiscard]] RenderPassHandle build();

    private:
        Device& _device;

        std::vector<VkAttachmentDescription2>   _attachments;
        std::vector<VkAttachmentReference2>     _color_attachments_refs;
        std::optional<VkAttachmentReference2>   _depth_attachment_ref;
    };
}
