//
//  DeviceQueue.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DeviceQueue_hpp
#define DeviceQueue_hpp

#include "Surface.hpp"
#include "CommandBuffer.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    class Device;
    
    class DeviceQueue
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device    указатель на устройство.
         * @param family_index  индекс семейства очередей.
         * @param index         индекс очереди в семействе очередей.
        */
        DeviceQueue(const PtrDevice& ptr_device, uint32_t family_index, uint32_t index);

        inline DeviceQueue(DeviceQueue&& device_queue);
        DeviceQueue(const DeviceQueue&) = delete;

        inline ~DeviceQueue();

        DeviceQueue& operator = (DeviceQueue&&)         = delete;
        DeviceQueue& operator = (const DeviceQueue&)    = delete;

        inline void submit(const CommandBuffer& command_buffer);

        inline PtrDevice&           getDevice()         noexcept;
        inline const PtrDevice&     getDevice()         const noexcept; 
        inline const VkQueue&       getQueueHandle()    const noexcept;
        inline uint32_t             getFamilyIndex()    const noexcept;
        inline uint32_t             getIndex()          const noexcept;

        /**
         * @brief Метод позволяющий показать пользователю изображение.
         * 
         * @param ptr_swapchain указатель на список показа.
         * @param image_index   индекс изображения в списке показа.
        */
        inline void setPresent(
            const PtrSwapchain& ptr_swapchain,
            uint32_t            image_index
        );

        /**
         * @brief Метод позволяющий показать пользователю изображение.
         * 
         * @param swapchain     список показа.
         * @param image_index   индекс изображения в списке показа.
        */
        inline void setPresent(
            const Swapchain&    swapchain,
            uint32_t            image_index
        );

        /**
         * @brief Метод ожидающий пока все команды в очереди не будут выполнены.
        */
        inline void waitIdle() const;

        inline static bool isPresentSuppoerted(
            uint32_t                queue_family_index, 
            const PhysicalDevice&   physocal_device, 
            const Surface&          surface
        );

    private:
        PtrDevice   _ptr_device;
        VkQueue     _queue_handle;
        uint32_t    _family_index;
        uint32_t    _index;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline DeviceQueue::DeviceQueue(DeviceQueue&& device_queue) :
        _ptr_device     (device_queue._ptr_device),
        _queue_handle   (VK_NULL_HANDLE),
        _family_index   (device_queue._family_index),
        _index          (device_queue._index)
    {
        swap(_queue_handle, device_queue._queue_handle);
    }

    inline DeviceQueue::~DeviceQueue()
    {
        /// Деструктор должен завершиться только после того,
        /// как все команды в данной очереди будут выполнены.
        vkQueueWaitIdle(_queue_handle);
    }

   inline void DeviceQueue::submit(const CommandBuffer& command_buffer)
   {
       VkSubmitInfo submit_info {
           .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
           .commandBufferCount     = 1,
           .pCommandBuffers        = &command_buffer.getCommandBufferHandle()
       };

       assert(vkQueueSubmit(
           _queue_handle,
           1,
           &submit_info,
           VK_NULL_HANDLE
       ) == VK_SUCCESS);
   }

    inline PtrDevice& DeviceQueue::getDevice() noexcept
    {
        return _ptr_device;
    }

    inline const PtrDevice& DeviceQueue::getDevice() const noexcept
    {
        return _ptr_device;
    }

    inline const VkQueue& DeviceQueue::getQueueHandle() const noexcept
    {
        return _queue_handle;
    }

    inline uint32_t DeviceQueue::getFamilyIndex() const noexcept
    {
        return _family_index;
    }

    inline uint32_t DeviceQueue::getIndex() const noexcept
    {
        return _index;
    }

    inline void DeviceQueue::setPresent(
        const PtrSwapchain& ptr_swapchain,
        uint32_t            image_index
    )
    {
        VkPresentInfoKHR present_info {
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .swapchainCount     = 1,
            .pSwapchains        = &ptr_swapchain->getSwapchainHandle(),
            .pImageIndices      = &image_index,
        };

        assert(vkQueuePresentKHR(_queue_handle, &present_info) == VK_SUCCESS);
    }

    inline void DeviceQueue::setPresent(
        const Swapchain&    swapchain,
        uint32_t            image_index
    )
    {
        VkPresentInfoKHR present_info {
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .swapchainCount     = 1,
            .pSwapchains        = &swapchain.getSwapchainHandle(),
            .pImageIndices      = &image_index,
        };

        assert(vkQueuePresentKHR(_queue_handle, &present_info) == VK_SUCCESS);
    }

    inline void DeviceQueue::waitIdle() const
    {
        vkQueueWaitIdle(_queue_handle);
    }

    inline bool DeviceQueue::isPresentSuppoerted(
        uint32_t                queue_family_index, 
        const PhysicalDevice&   physocal_device, 
        const Surface&          surface
    )
    {
        VkBool32 res;

        vkGetPhysicalDeviceSurfaceSupportKHR(
            physocal_device.physical_device_handle, 
            queue_family_index, 
            surface.getSurfaceHandle(), 
            &res
        );
        
        return res == VK_TRUE;
    }
}

#endif /* DeviceQueue_hpp */
