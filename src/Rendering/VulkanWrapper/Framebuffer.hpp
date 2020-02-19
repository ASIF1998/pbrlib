//
//  Framebuffer.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 18/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include "Image.hpp"
#include "RenderPass.hpp"

namespace pbrlib
{
    class Framebuffer
    {
    public:
        inline Framebuffer(const shared_ptr<RenderPass>& ptr_render_pass,
                           vector<ImageView>&& attachments,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layers);

        inline Framebuffer(Framebuffer&& framebuffer);
        Framebuffer(const Framebuffer&) = delete;

        inline ~Framebuffer();

        Framebuffer operator = (Framebuffer&&) = delete;
        Framebuffer operator = (const Framebuffer&) = delete;

        inline shared_ptr<RenderPass>& getRenderPass() noexcept;    
        inline const shared_ptr<RenderPass>& getRenderPass() const noexcept;
        inline vector<ImageView>& getAttachments() noexcept;
        inline const vector<ImageView>& getAttachments() const noexcept;
        inline uint32_t getWidth() const noexcept;
        inline uint32_t getHeight() const noexcept;
        inline uint32_t getLayers() const noexcept;

    private:
        shared_ptr<RenderPass> _ptr_render_pass;
        vector<ImageView> _attachments;
        VkFramebuffer _framebuffer_handle;
        uint32_t _width;
        uint32_t _height;
        uint32_t _layers;
    };
}

#include "Framebuffer.inl"

#endif /* Framebuffer_hpp */
