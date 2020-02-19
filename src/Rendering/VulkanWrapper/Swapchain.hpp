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

#include "DeviceQueue.hpp"

#include <numeric>

namespace pbrlib
{
    class Swapchain
    {
    public:
        /**
         * @brief Конструктор.
         * @details 
         *      При использовании данного конструктора изображения 
         *      можно будет использовать несколькими очередями.
         * 
         * @param ptr_device указатель на устройство.
         * @param queue_family_indices индексы семейства очередей.
         * @param surface поверхность.
        */
        inline Swapchain(const shared_ptr<Device>& ptr_device,
                         vector<uint32_t> queue_family_indices,
                         const shared_ptr<Surface>& surface);
        
        /**
         * @brief Конструктор.
         * @details 
         *      При использовании данного конструктора изображения 
         *      можно будет использовать только на одной очереди.
         * 
         * @param ptr_device указатель на устройство.
         * @param queue_family_index индекс семейства очередей.
         * @param surface поверхность.
        */
        inline Swapchain(const shared_ptr<Device>& ptr_device,
                         uint32_t queue_family_index,
                         const shared_ptr<Surface>& surface);

        inline Swapchain(Swapchain&& swapchain);
        Swapchain(const Swapchain&) = delete;

        inline ~Swapchain();

        Swapchain& operator = (Swapchain&&) = delete;
        Swapchain& operator = (const Swapchain&) = delete;

        /**
         * @brief Функция создающая список показа.
         * 
         * @param ptr_device указатель на устройство.
         * @param queue_family_indices индексы семейства очередей.
         * @param sharing_mode сообщает о том как изображения будут использоваться в разных очередях.
        */
        void create(const shared_ptr<Device>& ptr_device, 
                    vector<uint32_t> queue_family_indices,
                    VkSharingMode sharing_mode);

        inline void setPresent(DeviceQueue& queue, uint32_t image_index,  const VkSemaphore* semaphores, uint32_t semaphore_count);

        inline vector<ImageView>& getImagesView() noexcept;
        inline const vector<ImageView>& getImagesView() const noexcept;
        inline void getNextPresentImageIndex(uint32_t& image_index, VkSemaphore semaphore, VkFence fence = VK_NULL_HANDLE);
        inline VkSwapchainKHR getSwapchainHandle() const noexcept;
        inline shared_ptr<Surface>& getSurface() noexcept;
        inline const shared_ptr<Surface>& getSurface() const noexcept;

        /**
         * @brief Статический метод создающий объект типа shared_ptr<Swapchain>.
         * 
         * @param ptr_device указатель на устройство.
         * @param queue_family_indices индексы семейства очередей.
         * @param surface поверхность.
        */
        inline static shared_ptr<Swapchain> makeSwapchain(const shared_ptr<Device>& ptr_device,
                                                          vector<uint32_t> queue_family_indices,
                                                          const shared_ptr<Surface>& surface);

        /**
         * @brief Статический метод создающий объект типа shared_ptr<Swapchain>.
         * 
         * @param ptr_device указатель на устройство.
         * @param queue_family_index индекс семейства очередей.
         * @param surface поверхность.
        */
        inline static shared_ptr<Swapchain> makeSwapchain(const shared_ptr<Device>& ptr_device,
                                                          uint32_t queue_family_index,
                                                          const shared_ptr<Surface>& surface);

    private:
        VkSwapchainKHR _swapchain_handle;
        shared_ptr<Surface> _ptr_surface;
        vector<ImageView> _images_view;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Swapchain::Swapchain(const shared_ptr<Device>& ptr_device,
                         vector<uint32_t> queue_family_indices,
                         const shared_ptr<Surface> & surface) :
        _swapchain_handle(VK_NULL_HANDLE),
        _ptr_surface(surface)
    {
        create(ptr_device, queue_family_indices, VK_SHARING_MODE_CONCURRENT);
    }

    inline Swapchain::Swapchain(const shared_ptr<Device>& ptr_device,
                         uint32_t queue_family_index,
                         const shared_ptr<Surface> & surface) :
        _swapchain_handle(VK_NULL_HANDLE),
        _ptr_surface(surface)
    {
        vector<uint32_t> queue_family_indicies {queue_family_index};
        create(ptr_device, queue_family_indicies, VK_SHARING_MODE_EXCLUSIVE);
    }

    inline Swapchain:: Swapchain(Swapchain&& swapchain) :
        _swapchain_handle(VK_NULL_HANDLE),
        _ptr_surface(swapchain._ptr_surface),
        _images_view(move(swapchain._images_view))
    {
        swap(_swapchain_handle, swapchain._swapchain_handle);
    }

    inline Swapchain::~Swapchain()
    {
        if (_swapchain_handle != VK_NULL_HANDLE) {
            for (size_t i{0}; i < _images_view.size(); i++) {
                vkDestroyImageView(_images_view[i].getImage()->getDevice()->getDeviceHandle(), _images_view[i]._image_view_handle, nullptr);
                _images_view[i]._image_view_handle = VK_NULL_HANDLE;
                _images_view[i]._ptr_image->_image_handle = VK_NULL_HANDLE;
            }
            
            vkDestroySwapchainKHR(_images_view[0]._ptr_image->getDevice()->getDeviceHandle(), _swapchain_handle, nullptr);
        }
    }

    inline void Swapchain::setPresent(DeviceQueue& queue, uint32_t image_index,  const VkSemaphore* semaphores, uint32_t semaphore_count)
    {
        VkPresentInfoKHR present_info {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = semaphore_count,
            .pWaitSemaphores = semaphores,
            .swapchainCount = 1,
            .pSwapchains = &_swapchain_handle,
            .pImageIndices = &image_index,
            .pResults = nullptr
        };

        assert(vkQueuePresentKHR(queue.getQueueHandle(), &present_info) == VK_SUCCESS);
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

    inline VkSwapchainKHR Swapchain::getSwapchainHandle() const noexcept
    {
        return _swapchain_handle;
    }

    inline shared_ptr<Surface>& Swapchain::getSurface() noexcept
    {
        return _ptr_surface;
    }

    inline const shared_ptr<Surface>& Swapchain::getSurface() const noexcept
    {
        return _ptr_surface;
    }

    inline shared_ptr<Swapchain> Swapchain::makeSwapchain(const shared_ptr<Device>& ptr_device,
                                                          vector<uint32_t> queue_family_indices,
                                                          const shared_ptr<Surface>& surface)
    {
        return make_shared<Swapchain>(ptr_device, queue_family_indices, surface);
    }

    inline shared_ptr<Swapchain> Swapchain::makeSwapchain(const shared_ptr<Device>& ptr_device,
                                                          uint32_t queue_family_index,
                                                          const shared_ptr<Surface>& surface)
    {
        return make_shared<Swapchain>(ptr_device, queue_family_index, surface);
    }
}

#endif /* Swapchain_hpp */
