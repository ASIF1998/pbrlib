//
//  Framebuffer.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 18/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline Framebuffer::Framebuffer(
        const PtrSwapchain&     ptr_swapchain,
        uint32_t                swapchain_attachment_indx,
        const PtrRenderPass&    ptr_render_pass,
        PtrAttachments&&        attachments,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    ) :
        _ptr_swapchain              (ptr_swapchain),
        _ptr_render_pass            (ptr_render_pass),
        _swapchain_attachment_indx  (swapchain_attachment_indx),
        _attachments                (move(attachments)),
        _framebuffer_handle         (VK_NULL_HANDLE),
        _width                      (width),
        _height                     (height),
        _layers                     (layers)
    {
        _create();
    }

    inline Framebuffer::Framebuffer(
        const PtrSwapchain&     ptr_swapchain,
        uint32_t                swapchain_attachment_indx,
        const PtrRenderPass&    ptr_render_pass,
        const PtrAttachments&   attachments,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    ) :
        _ptr_swapchain              (ptr_swapchain),
        _ptr_render_pass            (ptr_render_pass),
        _swapchain_attachment_indx  (swapchain_attachment_indx),
        _attachments                (move(attachments)),
        _framebuffer_handle         (VK_NULL_HANDLE),
        _width                      (width),
        _height                     (height),
        _layers                     (layers)
    {
        _create();
    }

    inline Framebuffer::Framebuffer(
        const PtrSwapchain&     ptr_swapchain,
        uint32_t                swapchain_attachment_indx,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    ) :
        _ptr_swapchain              (ptr_swapchain),
        _ptr_render_pass            (ptr_render_pass),
        _attachments                (nullptr),
        _swapchain_attachment_indx  (swapchain_attachment_indx),
        _framebuffer_handle         (VK_NULL_HANDLE),
        _width                      (width),
        _height                     (height),
        _layers                     (layers)
    {
        _create();
    }

    inline Framebuffer::Framebuffer(
        PtrAttachments&&        attachments,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    ) :
        _ptr_swapchain              (nullptr),
        _ptr_render_pass            (ptr_render_pass),
        _swapchain_attachment_indx  (0),
        _attachments                (move(attachments)),
        _framebuffer_handle         (VK_NULL_HANDLE),
        _width                      (width),
        _height                     (height),
        _layers                     (layers)
    {
        _create();
    }

    inline Framebuffer::Framebuffer(
        const PtrAttachments&   attachments,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    ) :
        _ptr_swapchain              (nullptr),
        _ptr_render_pass            (ptr_render_pass),
        _swapchain_attachment_indx  (0),
        _attachments                (move(attachments)),
        _framebuffer_handle         (VK_NULL_HANDLE),
        _width                      (width),
        _height                     (height),
        _layers                     (layers)
    {
        _create();
    }

    inline Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
        _ptr_swapchain              (move(framebuffer._ptr_swapchain)),
        _ptr_render_pass            (move(framebuffer._ptr_render_pass)),
        _swapchain_attachment_indx  (framebuffer._swapchain_attachment_indx),
        _attachments                (move(framebuffer._attachments)),
        _framebuffer_handle         (VK_NULL_HANDLE),
        _width                      (framebuffer._width),
        _height                     (framebuffer._height),
        _layers                     (framebuffer._layers)
    {
        swap(_framebuffer_handle, framebuffer._framebuffer_handle);
    }

    inline Framebuffer::~Framebuffer()
    {
        if (_framebuffer_handle != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(_ptr_render_pass->getDevice()->getDeviceHandle(), _framebuffer_handle, nullptr);
        }
    }

    inline void Framebuffer::_create()
    {
        assert(_width && _height && _layers && (_attachments != nullptr || isUsedSwapchain()));

        vector<VkImageView> attachments;

        if (isUsedSwapchain()) {
            attachments.push_back(_ptr_swapchain->getImagesView()[_swapchain_attachment_indx].getImageViewHandle());
        }

        if (_attachments != nullptr) {
            for (size_t i{0}; i < _attachments->size(); i++) {
                attachments.push_back(_attachments->at(i).getImageViewHandle());
            }
        }

        VkFramebufferCreateInfo framebuffer_info = {
            .sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .renderPass         = _ptr_render_pass->getRenderPassHandle(),
            .attachmentCount    = static_cast<uint32_t>(attachments.size()),
            .pAttachments       = attachments.data(),
            .width              = _width,
            .height             = _height,
            .layers             = _layers
        };

        assert(vkCreateFramebuffer(
            _ptr_render_pass->getDevice()->getDeviceHandle(), 
            &framebuffer_info, 
            nullptr, 
            &_framebuffer_handle
        ) == VK_SUCCESS);
        
        assert(_framebuffer_handle != VK_NULL_HANDLE);
    }  

    inline bool Framebuffer::isUsedSwapchain() const noexcept     
    {
        return _ptr_swapchain != nullptr;
    }

    inline PtrSwapchain& Framebuffer::getSwapchain() noexcept
    {
        return _ptr_swapchain;
    }

    inline const PtrSwapchain& Framebuffer::getSwapchain() const noexcept
    {
        return _ptr_swapchain;
    }

    inline PtrRenderPass& Framebuffer::getRenderPass() noexcept
    {
        return _ptr_render_pass;
    }

    inline const PtrRenderPass& Framebuffer::getRenderPass() const noexcept
    {
        return _ptr_render_pass;
    }

    inline PtrAttachments& Framebuffer::getAttachments() noexcept
    {
        return _attachments;
    }

    inline const PtrAttachments& Framebuffer::getAttachments() const noexcept
    {
        return _attachments;
    }

    inline ImageView& Framebuffer::getSwapchainAttachment() noexcept
    {
        assert(_ptr_swapchain != nullptr);
        return _ptr_swapchain->getImagesView()[_swapchain_attachment_indx];
    }

    inline const ImageView& Framebuffer::getSwapchainAttachment() const noexcept
    {
        assert(_ptr_swapchain != nullptr);
        return _ptr_swapchain->getImagesView()[_swapchain_attachment_indx];
    }

    inline uint32_t Framebuffer::getSwapchainAttachmentIndex() const noexcept
    {
        return _swapchain_attachment_indx;
    }

    inline const VkFramebuffer& Framebuffer::getFramebufferHandle() const noexcept
    {
        return _framebuffer_handle;
    }

    inline uint32_t Framebuffer::getWidth() const noexcept
    {
        return _width;
    }

    inline uint32_t Framebuffer::getHeight() const noexcept
    {
        return _height;
    }

    inline uint32_t Framebuffer::getNumLayers() const noexcept
    {
        return _layers;
    }

    inline PtrDevice&  Framebuffer::getDevice() noexcept
    {
        return _ptr_render_pass->getDevice();
    }

    inline const PtrDevice& Framebuffer::getDevice() const noexcept
    {
        return _ptr_render_pass->getDevice();
    }

    inline PtrFramebuffer Framebuffer::make(
        const PtrSwapchain&     ptr_swapchain,
        uint32_t                swapchain_attachment_indx,
        const PtrRenderPass&    ptr_render_pass,
        PtrAttachments&&        attachments,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    )
    {
        return make_shared<Framebuffer>(
            ptr_swapchain, 
            swapchain_attachment_indx, 
            ptr_render_pass, 
            move(attachments), 
            width, 
            height, 
            layers
        );
    }

    inline PtrFramebuffer Framebuffer::make(
        const PtrSwapchain&     ptr_swapchain,
        uint32_t                swapchain_attachment_indx,
        const PtrRenderPass&    ptr_render_pass,
        const PtrAttachments&   attachments,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    )
    {
        return make_shared<Framebuffer>(

            ptr_swapchain,
            swapchain_attachment_indx,
            ptr_render_pass, 
            attachments,
            width,
            height,
            layers
        );
    }

    inline PtrFramebuffer Framebuffer::make(
        const PtrSwapchain&     ptr_swapchain, 
        uint32_t                swapchain_attachment_indx,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    )
    {
        return make_shared<Framebuffer>(
            ptr_swapchain,
            swapchain_attachment_indx,
            ptr_render_pass,
            width,
            height,
            layers
        );
    }

    inline PtrFramebuffer Framebuffer::make(
        PtrAttachments&&        attachments,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    )
    {
        return make_shared<Framebuffer>(
            move(attachments),
            ptr_render_pass,
            width,
            height,
            layers
        );
    }

    inline PtrFramebuffer Framebuffer::make(
        const PtrAttachments&   attachments,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    )
    {
        return make_shared<Framebuffer>(
            attachments,
            ptr_render_pass,
            width,
            height,
            layers
        );
    }
}
