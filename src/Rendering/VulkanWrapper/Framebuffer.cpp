//
//  Framebuffer.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 18/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/Framebuffer.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Image.hpp>

using namespace std;

namespace pbrlib
{
    Framebuffer::Framebuffer(
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

    Framebuffer::Framebuffer(
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

    Framebuffer::Framebuffer(
        const PtrSwapchain&     ptr_swapchain,
        uint32_t                swapchain_attachment_indx,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height,
        uint32_t                layers
    ) :
        _ptr_swapchain              (ptr_swapchain),
        _ptr_render_pass            (ptr_render_pass),
        _swapchain_attachment_indx  (swapchain_attachment_indx),
        _attachments                (nullptr),
        _framebuffer_handle         (VK_NULL_HANDLE),
        _width                      (width),
        _height                     (height),
        _layers                     (layers)
    {
        _create();
    }

    Framebuffer::Framebuffer(
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

    Framebuffer::Framebuffer(
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

    Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
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

    Framebuffer::~Framebuffer()
    {
        if (_framebuffer_handle != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(_ptr_render_pass->getDevice()->getDeviceHandle(), _framebuffer_handle, nullptr);
        }
    }

    void Framebuffer::_create()
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

        VkFramebufferCreateInfo framebuffer_info = { };
        framebuffer_info.sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass         = _ptr_render_pass->getRenderPassHandle();
        framebuffer_info.attachmentCount    = static_cast<uint32_t>(attachments.size());
        framebuffer_info.pAttachments       = attachments.data();
        framebuffer_info.width              = _width;
        framebuffer_info.height             = _height;
        framebuffer_info.layers             = _layers;

        assert(vkCreateFramebuffer(
            _ptr_render_pass->getDevice()->getDeviceHandle(), 
            &framebuffer_info, 
            nullptr, 
            &_framebuffer_handle
        ) == VK_SUCCESS);
        
        assert(_framebuffer_handle != VK_NULL_HANDLE);
    }  

    bool Framebuffer::isUsedSwapchain() const noexcept     
    {
        return _ptr_swapchain != nullptr;
    }

    PtrSwapchain& Framebuffer::getSwapchain() noexcept
    {
        return _ptr_swapchain;
    }

    const PtrSwapchain& Framebuffer::getSwapchain() const noexcept
    {
        return _ptr_swapchain;
    }

    PtrRenderPass& Framebuffer::getRenderPass() noexcept
    {
        return _ptr_render_pass;
    }

    const PtrRenderPass& Framebuffer::getRenderPass() const noexcept
    {
        return _ptr_render_pass;
    }

    PtrAttachments& Framebuffer::getAttachments() noexcept
    {
        return _attachments;
    }

    const PtrAttachments& Framebuffer::getAttachments() const noexcept
    {
        return _attachments;
    }

    ImageView& Framebuffer::getSwapchainAttachment() noexcept
    {
        assert(_ptr_swapchain != nullptr);
        return _ptr_swapchain->getImagesView()[_swapchain_attachment_indx];
    }

    const ImageView& Framebuffer::getSwapchainAttachment() const noexcept
    {
        assert(_ptr_swapchain != nullptr);
        return _ptr_swapchain->getImagesView()[_swapchain_attachment_indx];
    }

    uint32_t Framebuffer::getSwapchainAttachmentIndex() const noexcept
    {
        return _swapchain_attachment_indx;
    }

    const VkFramebuffer& Framebuffer::getFramebufferHandle() const noexcept
    {
        return _framebuffer_handle;
    }

    uint32_t Framebuffer::getWidth() const noexcept
    {
        return _width;
    }

    uint32_t Framebuffer::getHeight() const noexcept
    {
        return _height;
    }

    uint32_t Framebuffer::getNumLayers() const noexcept
    {
        return _layers;
    }

    PtrDevice&  Framebuffer::getDevice() noexcept
    {
        return _ptr_render_pass->getDevice();
    }

    const PtrDevice& Framebuffer::getDevice() const noexcept
    {
        return _ptr_render_pass->getDevice();
    }

    PtrFramebuffer Framebuffer::make(
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

    PtrFramebuffer Framebuffer::make(
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

    PtrFramebuffer Framebuffer::make(
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

    PtrFramebuffer Framebuffer::make(
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

    PtrFramebuffer Framebuffer::make(
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

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Framebuffer::Builder::Builder() :
        _ptr_swapchain              (nullptr),
        _ptr_render_pass            (nullptr),
        _ptr_attachments            (nullptr),
        _swapchain_attachment_index (0),
        _width                      (0),
        _height                     (0),
        _layers                     (0)
    {}

    void Framebuffer::Builder::setSwapchain(const PtrSwapchain& ptr_swapchain)
    {
        _ptr_swapchain = ptr_swapchain;
    }

    void Framebuffer::Builder::setSwapchainAttachmentIndex(uint32_t swapchain_attachment_index)
    {
        _swapchain_attachment_index = swapchain_attachment_index;
    }

    void Framebuffer::Builder::setRenderPass(const PtrRenderPass& ptr_render_pass)
    {
        _ptr_render_pass = ptr_render_pass;
    }

    void Framebuffer::Builder::setAttachments(const PtrAttachments& ptr_attachments)
    {
        _ptr_attachments = ptr_attachments;
    }

    void Framebuffer::Builder::setWidth(uint32_t width) noexcept
    {
        _width = width;
    }

    void Framebuffer::Builder::setHeight(uint32_t height) noexcept
    {
        _height = height;
    }

    void Framebuffer::Builder::setNumLayers(uint32_t num_layers) noexcept
    {
        _layers = num_layers;
    }

    Framebuffer Framebuffer::Builder::build() const
    {
        if (!_ptr_swapchain) {
            return Framebuffer(
                _ptr_attachments,
                _ptr_render_pass,
                _width,
                _height,
                _layers
            );
        }

        if (!_ptr_attachments) {
            return Framebuffer(
                _ptr_swapchain,
                _swapchain_attachment_index,
                _ptr_render_pass,
                _width,
                _height,
                _layers
            );
        }

        return Framebuffer(
            _ptr_swapchain,
            _swapchain_attachment_index,
            _ptr_render_pass,
            _ptr_attachments,
            _width,
            _height,
            _layers
        );
    }

    PtrFramebuffer Framebuffer::Builder::buildPtr() const
    {
        if (!_ptr_swapchain) {
            return make_shared<Framebuffer>(
                _ptr_attachments,
                _ptr_render_pass,
                _width,
                _height,
                _layers
            );
        }

        if (!_ptr_attachments) {
            return make_shared<Framebuffer>(
                _ptr_swapchain,
                _swapchain_attachment_index,
                _ptr_render_pass,
                _width,
                _height,
                _layers
            );
        }

        return make_shared<Framebuffer>(
            _ptr_swapchain,
            _swapchain_attachment_index,
            _ptr_render_pass,
            _ptr_attachments,
            _width,
            _height,
            _layers
        );
    }
}
