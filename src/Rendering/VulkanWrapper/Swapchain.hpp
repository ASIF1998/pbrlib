//
//  Swapchain.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Swapchain_hpp
#define Swapchain_hpp

#include "Image.hpp"
#include "Surface.hpp"

//#include "DeviceQueue.hpp"

#include <numeric>

namespace pbrlib
{
    class Swapchain;
    class DeviceQueue;

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
        inline Swapchain(
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
        inline Swapchain(
            const PtrDevice&            ptr_device,
            uint32_t                    queue_family_index,
            const PtrSurface&           surface
        );

        inline Swapchain(Swapchain&& swapchain);
        Swapchain(const Swapchain&) = delete;

        inline ~Swapchain();

        Swapchain& operator = (Swapchain&&)         = delete;
        Swapchain& operator = (const Swapchain&)    = delete;

        inline vector<ImageView>&           getImagesView()         noexcept;
        inline const vector<ImageView>&     getImagesView()         const noexcept;
        inline const VkSwapchainKHR&        getSwapchainHandle()    const noexcept;
        inline PtrSurface&                  getSurface()            noexcept;
        inline const PtrSurface&            getSurface()            const noexcept;
        inline PtrDevice&                   getDevice()             noexcept;
        inline const PtrDevice&             getDevice()             const noexcept;

        inline void getNextPresentImageIndex(
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
        inline static PtrSwapchain make(
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
        inline static PtrSwapchain make(
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

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Swapchain::Swapchain(
        const PtrDevice&            ptr_device,
        const vector<uint32_t>&     queue_family_indices,
        const PtrSurface&           surface
    ) :
        _swapchain_handle(VK_NULL_HANDLE),
        _ptr_surface(surface)
    {
        _create(ptr_device, queue_family_indices, VK_SHARING_MODE_CONCURRENT);
    }

    inline Swapchain::Swapchain(
        const PtrDevice&            ptr_device,
        uint32_t                    queue_family_index,
        const PtrSurface& surface
    ) :
        _swapchain_handle   (VK_NULL_HANDLE),
        _ptr_surface        (surface)
    {
        vector<uint32_t> queue_family_indicies {queue_family_index};
        _create(ptr_device, queue_family_indicies, VK_SHARING_MODE_EXCLUSIVE);
    }

    inline Swapchain:: Swapchain(Swapchain&& swapchain) :
        _swapchain_handle   (VK_NULL_HANDLE),
        _ptr_surface        (move(swapchain._ptr_surface)),
        _images_view        (move(swapchain._images_view))
    {
        swap(_swapchain_handle, swapchain._swapchain_handle);
    }

    inline Swapchain::~Swapchain()
    {
        if (_swapchain_handle != VK_NULL_HANDLE) {
            for (size_t i{0}; i < _images_view.size(); i++) {
                vkDestroyImageView(_images_view[i].getImage()->getDevice()->getDeviceHandle(), _images_view[i]._image_view_handle, nullptr);

                _images_view[i]._image_view_handle          = VK_NULL_HANDLE;
                _images_view[i]._ptr_image->_image_handle   = VK_NULL_HANDLE;
            }
            
            vkDestroySwapchainKHR(_images_view[0]._ptr_image->getDevice()->getDeviceHandle(), _swapchain_handle, nullptr);
        }
    }

    inline vector<ImageView>& Swapchain::getImagesView() noexcept
    {
        return _images_view;
    }


    inline const vector<ImageView>& Swapchain::getImagesView() const noexcept
    {
        return _images_view;
    }

    inline void Swapchain::getNextPresentImageIndex(uint32_t& image_index, VkSemaphore semaphore, VkFence fence)
    {
        VkDevice device_handle = _images_view[0].getImage()->getDevice()->getDeviceHandle();

        vkAcquireNextImageKHR(device_handle, _swapchain_handle, numeric_limits<uint64_t>::max(), semaphore, fence, &image_index);
    }

    inline const VkSwapchainKHR& Swapchain::getSwapchainHandle() const noexcept
    {
        return _swapchain_handle;
    }

    inline PtrSurface& Swapchain::getSurface() noexcept
    {
        return _ptr_surface;
    }

    inline const PtrSurface& Swapchain::getSurface() const noexcept
    {
        return _ptr_surface;
    }

    inline PtrDevice& Swapchain::getDevice() noexcept
    {
        return _images_view[0].getImage()->getDevice();
    }
    inline const PtrDevice& Swapchain::getDevice() const noexcept
    {
        return _images_view[0].getImage()->getDevice();
    }

    inline PtrSwapchain Swapchain::make(
        const PtrDevice&   ptr_device,
        vector<uint32_t>            queue_family_indices,
        const shared_ptr<Surface>&  surface
    )
    {
        return make_shared<Swapchain>(ptr_device, queue_family_indices, surface);
    }

    inline PtrSwapchain Swapchain::make(
        const PtrDevice&   ptr_device,
        uint32_t                    queue_family_index,
        const shared_ptr<Surface>&  surface
    )
    {
        return make_shared<Swapchain>(ptr_device, queue_family_index, surface);
    }
}

#endif /* Swapchain_hpp */
