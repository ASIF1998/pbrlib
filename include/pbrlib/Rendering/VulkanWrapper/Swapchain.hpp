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

#include <span>

namespace pbrlib
{
    class DeviceQueue;
    class ImageView;
    class Device;

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
            const Device*                   ptr_device,
            std::span<const uint32_t>       queue_family_indices,
            std::shared_ptr<const Surface>  surface
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
            const Device*                   ptr_device,
            uint32_t                        queue_family_index,
            std::shared_ptr<const Surface>  surface
        );

        Swapchain(Swapchain&& swapchain);
        Swapchain(const Swapchain&) = delete;

        ~Swapchain();

        Swapchain& operator = (Swapchain&&)         = delete;
        Swapchain& operator = (const Swapchain&)    = delete;

        std::vector<ImageView>&         getImagesView()         noexcept;
        const std::vector<ImageView>&   getImagesView()         const noexcept;
        const VkSwapchainKHR&           getSwapchainHandle()    const noexcept;
        std::shared_ptr<const Surface>  getSurface()            const noexcept;
        const Device*                   getDevice()             const noexcept;

        void getNextPresentImageIndex(
            uint32_t&   image_index, 
            VkSemaphore semaphore, 
            VkFence     fence = VK_NULL_HANDLE
        ) const;

        /**
         * @brief Статический метод создающий объект типа PtrSwapchain.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_indices  индексы семейства очередей.
         * @param surface               поверхность.
        */
        static std::unique_ptr<Swapchain> make(
            const Device*                   ptr_device,
            std::span<const uint32_t>       queue_family_indices,
            std::shared_ptr<const Surface>  surface
        );

        /**
         * @brief Статический метод создающий объект типа PtrSwapchain.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_index    индекс семейства очередей.
         * @param surface               поверхность.
        */
        static std::unique_ptr<Swapchain> make(
            const Device*                   ptr_device,
            uint32_t                        queue_family_index,
            std::shared_ptr<const Surface>  surface
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
            const Device*               ptr_device, 
            std::span<const uint32_t>   queue_family_indices,
            VkSharingMode               sharing_mode
        );

    private:
        VkSwapchainKHR                  _swapchain_handle;
        std::shared_ptr<const Surface>  _ptr_surface;
        std::vector<ImageView>          _images_view;
    };
}

#endif /* Swapchain_hpp */
