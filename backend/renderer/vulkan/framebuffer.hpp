#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <vector>

namespace pbrlib::backend::vk
{
    class Image;
    class Device;
}

namespace pbrlib::backend::vk::builders
{
    class Framebuffer
    {
        void validate();

    public:
        Framebuffer(Device& device);

        Framebuffer(Framebuffer&& builder)      = delete;
        Framebuffer(const Framebuffer& builder) = delete;

        Framebuffer& operator = (Framebuffer&& builder)         = delete;
        Framebuffer& operator = (const Framebuffer& builder)    = delete;

        Framebuffer& renderPass(VkRenderPass render_pass_handle)    noexcept;
        Framebuffer& size(uint32_t width, uint32_t height)          noexcept;
        Framebuffer& layers(uint32_t layerCount)                    noexcept;

        Framebuffer& addAttachment(const vk::Image& attachment);

        [[nodiscard]] FramebufferHandle build();

    private:
        Device& _device;

        VkRenderPass _render_pass_handle = VK_NULL_HANDLE;

        std::vector<VkImageView> _attachments;

        uint32_t _width         = 0u;
        uint32_t _height        = 0u;
        uint32_t _layerCount    = 0u;
    };
}
