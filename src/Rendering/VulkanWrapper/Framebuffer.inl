//
//  Framebuffer.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 18/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline Framebuffer::Framebuffer(const shared_ptr<RenderPass>& ptr_render_pass,
                                    vector<ImageView>&& attachments,
                                    uint32_t width,
                                    uint32_t height,
                                    uint32_t layers) :
        _ptr_render_pass(ptr_render_pass),
        _attachments(move(attachments)),
        _framebuffer_handle(VK_NULL_HANDLE),
        _width(width),
        _height(height),
        _layers(layers)
    {
        vector<VkImageView> images_view (_attachments.size());

        for (size_t i{0}; i < images_view.size(); i++) {
            images_view[i] = _attachments[i].getImageViewHandle();
        }

        VkFramebufferCreateInfo framebuffer_info {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .renderPass = _ptr_render_pass->getRenderPassHandle(),
            .attachmentCount = static_cast<uint32_t>(images_view.size()),
            .pAttachments = images_view.data(),
            .width = _width,
            .height = _height,
            .layers = _layers
        };

        assert(vkCreateFramebuffer(_ptr_render_pass->getDevice()->getDeviceHandle(),
                                   &framebuffer_info,
                                   nullptr,
                                   &_framebuffer_handle) == VK_SUCCESS);
        assert(_framebuffer_handle != VK_NULL_HANDLE);
    }

    inline Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
        _ptr_render_pass(move(framebuffer._ptr_render_pass)),
        _attachments(move(framebuffer._attachments)),
        _framebuffer_handle(VK_NULL_HANDLE),
        _width(framebuffer._width),
        _height(framebuffer._height),
        _layers(framebuffer._layers)
    {
        swap(_framebuffer_handle, framebuffer._framebuffer_handle);
    }

    inline Framebuffer::~Framebuffer()
    {
        if (_framebuffer_handle != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(_ptr_render_pass->getDevice()->getDeviceHandle(), _framebuffer_handle, nullptr);
        }
    }

    inline shared_ptr<RenderPass>& Framebuffer::getRenderPass() noexcept
    {
        return _ptr_render_pass;
    }

    inline const shared_ptr<RenderPass>& Framebuffer::getRenderPass() const noexcept
    {
        return _ptr_render_pass;
    }

    inline vector<ImageView>& Framebuffer::getAttachments() noexcept
    {
        return _attachments;
    }

    inline const vector<ImageView>& Framebuffer::getAttachments() const noexcept
    {
        return _attachments;
    }

    inline uint32_t Framebuffer::getWidth() const noexcept
    {
        return _width;
    }

    inline uint32_t Framebuffer::getHeight() const noexcept
    {
        return _height;
    }

    inline uint32_t Framebuffer::getLayers() const noexcept
    {
        return _layers;
    }
}
