#include <backend/renderer/vulkan/framebuffer.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <backend/utils/vulkan.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::vk
{
    FramebufferBuild::FramebufferBuild(Device& device) :
        _device (device)
    { }

    FramebufferBuild& FramebufferBuild::renderPass(VkRenderPass render_pass_handle) noexcept
    {
        _render_pass_handle = render_pass_handle;
        return *this;
    }

    FramebufferBuild& FramebufferBuild::size(uint32_t width, uint32_t height) noexcept
    {
        _width  = width;
        _height = height;

        return *this;
    }

    FramebufferBuild& FramebufferBuild::layers(uint32_t layerCount) noexcept
    {
        _layerCount = layerCount;
        return *this;
    }

    FramebufferBuild& FramebufferBuild::addAttachment(const Image& attachment)
    {
        _attachments.push_back(attachment.view_handle);
        return *this;
    }

    void FramebufferBuild::validate()
    {
        if (_render_pass_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidState("[vk-framebuffer::bulder] render pass handle is null");

        if (_width == 0 || _height == 0) [[unlikely]]
            throw exception::InvalidState("[vk-framebuffer::bulder] size is 0");

        if (_layerCount == 0) [[unlikely]]
            throw exception::InvalidState("[vk-framebuffer::bulder] layer count is 0");

        if (_attachments.empty()) [[unlikely]]
            throw exception::InvalidState("[vk-framebuffer::bulder] no attachments");
    }

    VkFramebuffer FramebufferBuild::build()
    {
        validate();

        const VkFramebufferCreateInfo framebuffer_info =
        {
            .sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass         = _render_pass_handle,
            .attachmentCount    = static_cast<uint32_t>(_attachments.size()),
            .pAttachments       = _attachments.data(),
            .width              = _width,
            .height             = _height,
            .layers             = _layerCount
        };

        VkFramebuffer framebuffer_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateFramebuffer(
            _device.device(),
            &framebuffer_info,
            nullptr,
            &framebuffer_handle
        ));

        return framebuffer_handle;
    }
}