//
//  Swapchain.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Swapchain_hpp
#define Swapchain_hpp

#include "Surface.hpp"

#include <numeric>

namespace pbrlib
{
    class Swapchain;
    class DeviceQueue;
    class ImageView;

    using PtrSwapchain = shared_ptr<Swapchain>;

    class Swapchain
    {
    public:
        /**
         * @brief Конструктор.
         * @details 
         *      При использовании данного конструктора изображения 
         *      можно будет использовать несколькими очередями.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_indices  индексы семейства очередей.
         * @param surface               поверхность.
        */
        Swapchain(
            const PtrDevice&            ptr_device,
            const vector<uint32_t>&     queue_family_indices,
            const PtrSurface&           surface
        );
        
        /**
         * @brief Конструктор.
         * @details 
         *      При использовании данного конструктора изображения 
         *      можно будет использовать только на одной очереди.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_index    индекс семейства очередей.
         * @param surface               поверхность.
        */
        Swapchain(
            const PtrDevice&            ptr_device,
            uint32_t                    queue_family_index,
            const PtrSurface&           surface
        );

        Swapchain(Swapchain&& swapchain);
        Swapchain(const Swapchain&) = delete;

        ~Swapchain();

        Swapchain& operator = (Swapchain&&)         = delete;
        Swapchain& operator = (const Swapchain&)    = delete;

        vector<ImageView>&           getImagesView()         noexcept;
        const vector<ImageView>&     getImagesView()         const noexcept;
        const VkSwapchainKHR&        getSwapchainHandle()    const noexcept;
        PtrSurface&                  getSurface()            noexcept;
        const PtrSurface&            getSurface()            const noexcept;
        PtrDevice&                   getDevice()             noexcept;
        const PtrDevice&             getDevice()             const noexcept;

        void getNextPresentImageIndex(
            uint32_t&   image_index, 
            VkSemaphore semaphore, 
            VkFence     fence = VK_NULL_HANDLE
        );

        /**
         * @brief Статический метод создающий объект типа PtrSwapchain.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_indices  индексы семейства очередей.
         * @param surface               поверхность.
        */
        static PtrSwapchain make(
            const PtrDevice&            ptr_device,
            vector<uint32_t>            queue_family_indices,
            const shared_ptr<Surface>&  surface
        );

        /**
         * @brief Статический метод создающий объект типа PtrSwapchain.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_index    индекс семейства очередей.
         * @param surface               поверхность.
        */
        static PtrSwapchain make(
            const PtrDevice&            ptr_device,
            uint32_t                    queue_family_index,
            const shared_ptr<Surface>&  surface
        );

    private:
        /**
         * @brief Функция создающая список показа.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_indices  индексы семейства очередей.
         * @param sharing_mode          сообщает о том как изображения будут использоваться в разных очередях.
        */
        void _create(
            const PtrDevice&        ptr_device, 
            const vector<uint32_t>& queue_family_indices,
            VkSharingMode           sharing_mode
        );

    private:
        VkSwapchainKHR      _swapchain_handle;
        PtrSurface          _ptr_surface;
        vector<ImageView>   _images_view;
    };
}

#endif /* Swapchain_hpp */
