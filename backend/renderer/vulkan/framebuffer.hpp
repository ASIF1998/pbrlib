#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace pbrlib::backend::vk
{
    class Image;
    class Device;
}

namespace pbrlib::backend::vk
{
    class FramebufferBuild
    {
        void validate();

    public:
        FramebufferBuild(Device& device);

        FramebufferBuild(FramebufferBuild&& builder)        = delete;
        FramebufferBuild(const FramebufferBuild& builder)   = delete;

        FramebufferBuild& operator = (FramebufferBuild&& builder)       = delete;
        FramebufferBuild& operator = (const FramebufferBuild& builder)  = delete;

        FramebufferBuild& renderPass(VkRenderPass render_pass_handle)   noexcept;
        FramebufferBuild& size(uint32_t width, uint32_t height)         noexcept;
        FramebufferBuild& layers(uint32_t layerCount)                   noexcept;
        
        FramebufferBuild& addAttachment(const Image& attachment);

        [[nodiscard]] VkFramebuffer build();
        
    private:
        Device& _device;

        VkRenderPass _render_pass_handle = VK_NULL_HANDLE;

        std::vector<VkImageView> _attachments;

        uint32_t _width         = 0u;
        uint32_t _height        = 0u;
        uint32_t _layerCount    = 0u;
    };
}