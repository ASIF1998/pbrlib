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
#include "Swapchain.hpp"

namespace pbrlib
{
    using PtrAttachments = shared_ptr<vector<ImageView>>;

    class Framebuffer
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments иные виды изображения (например глубина-трафарет), 
         *                    которые, при создании фреймбуфера, будут идти после 
         *                    видов изображения в списке отображения.
         * @param width ширина фреймбуфера.
         * @param height высота фреймбуфера.
         * @param layers количество слоёв фреймбуфера.
        */
        inline Framebuffer(const shared_ptr<Swapchain>& ptr_swapchain,
                           uint32_t swapchain_attachment_indx,
                           const shared_ptr<RenderPass>& ptr_render_pass,
                           PtrAttachments&& attachments,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layers);

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments иные виды изображения (например глубина-трафарет), 
         *                    которые, при создании фреймбуфера, будут идти после 
         *                    видов изображения в списке отображения.
         * @param width ширина фреймбуфера.
         * @param height высота фреймбуфера.
         * @param layers количество слоёв фреймбуфера.
        */
        inline Framebuffer(const shared_ptr<Swapchain>& ptr_swapchain,
                           uint32_t swapchain_attachment_indx,
                           const shared_ptr<RenderPass>& ptr_render_pass,
                           const PtrAttachments& attachments,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layers);

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param width ширина фреймбуфера.
         * @param height высота фреймбуфера.
         * @param layers количество слоёв фреймбуфера.
        */  
        inline Framebuffer(const shared_ptr<Swapchain>& ptr_swapchain, 
                           uint32_t swapchain_attachment_indx,
                           const shared_ptr<RenderPass>& ptr_render_pass,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layers);

        /**
         * @brief Конструктор.
         * 
         * @param attachments виды изображений.
         * @param width ширина фреймбуфера.
         * @param height высота фреймбуфера.
         * @param layers количество слоёв фреймбуфера.
        */  
        inline Framebuffer(PtrAttachments&& attachments,
                           const shared_ptr<RenderPass>& ptr_render_pass,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layers);

        /**
         * @brief Конструктор.
         * 
         * @param attachments виды изображений.
         * @param width ширина фреймбуфера.
         * @param height высота фреймбуфера.
         * @param layers количество слоёв фреймбуфера.
        */  
        inline Framebuffer(const PtrAttachments& attachments,
                           const shared_ptr<RenderPass>& ptr_render_pass,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layers);

        inline Framebuffer(Framebuffer&& framebuffer);
        Framebuffer(const Framebuffer& framebuffer) = delete;

        inline ~Framebuffer();

        Framebuffer& operator = (Framebuffer&&) = delete;
        Framebuffer& operator = (const Framebuffer&) = delete;

        inline bool isUsedSwapchain() const noexcept;

        inline shared_ptr<Swapchain>& getSwapchain() noexcept;
        inline const shared_ptr<Swapchain>& getSwapchain() const noexcept;
        inline shared_ptr<RenderPass>& getRenderPass() noexcept;
        inline const shared_ptr<RenderPass>& getRenderPass() const noexcept;
        inline PtrAttachments& getAttachments() noexcept;
        inline const PtrAttachments& getAttachments() const noexcept;
        inline ImageView& getSwapchainAttachment() noexcept;
        inline const ImageView& getSwapchainAttachment() const noexcept;
        inline uint32_t getSwapchainAttachmentIndex() const noexcept;
        inline VkFramebuffer getFramebufferHandle() const noexcept;
        inline uint32_t getWidth() const noexcept;
        inline uint32_t getHeight() const noexcept;
        inline uint32_t getNumLayers() const noexcept;

    private:
        void _create_framebuffer();

    private:  
        shared_ptr<Swapchain> _ptr_swapchain;
        shared_ptr<RenderPass> _ptr_render_pass;
        uint32_t _swapchain_attachment_indx;
        PtrAttachments _attachments;
        VkFramebuffer _framebuffer_handle;
        uint32_t _width;
        uint32_t _height;
        uint32_t _layers;
    };
}

#include "Framebuffer.inl"

#endif /* Framebuffer_hpp */
