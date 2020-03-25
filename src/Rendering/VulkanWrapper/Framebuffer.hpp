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
    class Framebuffer;

    using PtrAttachments = shared_ptr<vector<ImageView>>;
    using PtrFramebuffer = shared_ptr<Framebuffer>;

    class Framebuffer
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        inline Framebuffer(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            PtrAttachments&&        attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        inline Framebuffer(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            const PtrAttachments&   attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */  
        inline Framebuffer(
            const PtrSwapchain&     ptr_swapchain, 
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        inline Framebuffer(
            PtrAttachments&&        attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        inline Framebuffer(
            const PtrAttachments&   attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        inline Framebuffer(Framebuffer&& framebuffer);
        Framebuffer(const Framebuffer& framebuffer) = delete;

        inline ~Framebuffer();

        Framebuffer& operator = (Framebuffer&&)         = delete;
        Framebuffer& operator = (const Framebuffer&)    = delete;

        inline bool isUsedSwapchain() const noexcept;

        inline PtrSwapchain&            getSwapchain()                  noexcept;
        inline const PtrSwapchain&      getSwapchain()                  const noexcept;
        inline PtrRenderPass&           getRenderPass()                 noexcept;
        inline const PtrRenderPass&     getRenderPass()                 const noexcept;
        inline PtrAttachments&          getAttachments()                noexcept;
        inline const PtrAttachments&    getAttachments()                const noexcept;
        inline ImageView&               getSwapchainAttachment()        noexcept;
        inline const ImageView&         getSwapchainAttachment()        const noexcept;
        inline uint32_t                 getSwapchainAttachmentIndex()   const noexcept;
        inline const VkFramebuffer&     getFramebufferHandle()          const noexcept;
        inline uint32_t                 getWidth()                      const noexcept;
        inline uint32_t                 getHeight()                     const noexcept;
        inline uint32_t                 getNumLayers()                  const noexcept;
        inline PtrDevice&               getDevice()                     noexcept;
        inline const PtrDevice&         getDevice()                     const noexcept;

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        inline static PtrFramebuffer make(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            PtrAttachments&&        attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        inline static PtrFramebuffer make(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            const PtrAttachments&   attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */  
        inline static PtrFramebuffer make(
            const PtrSwapchain&     ptr_swapchain, 
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        inline static PtrFramebuffer make(
            PtrAttachments&&        attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        inline static PtrFramebuffer make(
            const PtrAttachments&   attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

    private:
        inline void _create();

    private:  
        PtrSwapchain    _ptr_swapchain;
        PtrRenderPass   _ptr_render_pass;
        uint32_t        _swapchain_attachment_indx;
        PtrAttachments  _attachments;
        VkFramebuffer   _framebuffer_handle;
        uint32_t        _width;
        uint32_t        _height;
        uint32_t        _layers;
    };
}

#include "Framebuffer.inl"

#endif /* Framebuffer_hpp */
