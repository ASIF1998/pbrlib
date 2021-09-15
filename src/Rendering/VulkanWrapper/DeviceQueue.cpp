//
//  DeviceQueue.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "DeviceQueue.hpp"
#include "Device.hpp"

#include "PhysicalDevice.hpp"

#include "CommandBuffer.hpp"

#include <cassert>

namespace pbrlib
{
    DeviceQueue::DeviceQueue(
        const PtrDevice&            ptr_device, 
        uint32_t                    family_index, 
        uint32_t                    index
    ) :
        _ptr_device     (ptr_device),
        _queue_handle   (VK_NULL_HANDLE),
        _family_index   (family_index),
        _index          (index)
    {
        vkGetDeviceQueue(_ptr_device->getDeviceHandle(), _family_index, _index, &_queue_handle);
        assert(_queue_handle != VK_NULL_HANDLE);
    }

    DeviceQueue::DeviceQueue(DeviceQueue&& device_queue) :
        _ptr_device     (move(device_queue._ptr_device)),
        _queue_handle   (VK_NULL_HANDLE),
        _family_index   (device_queue._family_index),
        _index          (device_queue._index)
    {
        swap(_queue_handle, device_queue._queue_handle);
    }

    DeviceQueue::~DeviceQueue()
    {
        /// Деструктор должен завершиться только после того,
        /// как все команды в данной очереди будут выполнены.
        vkQueueWaitIdle(_queue_handle);
    }

   void DeviceQueue::submit(const CommandBuffer& command_buffer)
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

    PtrDevice& DeviceQueue::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& DeviceQueue::getDevice() const noexcept
    {
        return _ptr_device;
    }

    const VkQueue& DeviceQueue::getQueueHandle() const noexcept
    {
        return _queue_handle;
    }

    uint32_t DeviceQueue::getFamilyIndex() const noexcept
    {
        return _family_index;
    }

    uint32_t DeviceQueue::getIndex() const noexcept
    {
        return _index;
    }

    void DeviceQueue::setPresent(
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

    void DeviceQueue::setPresent(
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

    void DeviceQueue::waitIdle() const
    {
        vkQueueWaitIdle(_queue_handle);
    }

    bool DeviceQueue::isPresentSuppoerted(
        uint32_t                queue_family_index, 
        const PhysicalDevice&   physical_device,
        const Surface&          surface
    )
    {
        VkBool32 res;

        vkGetPhysicalDeviceSurfaceSupportKHR(
            physical_device.physical_device_handle,
            queue_family_index, 
            surface.getSurfaceHandle(), 
            &res
        );
        
        return res == VK_TRUE;
    }

    PtrDeviceQueue DeviceQueue::make(const PtrDevice& ptr_device, uint32_t family_index, uint32_t index)
    {
        return make_shared<DeviceQueue>(ptr_device, family_index, index);
    }
}
