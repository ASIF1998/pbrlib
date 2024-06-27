//
//  DeviceQueue.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/DeviceQueue.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Device.hpp>

#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <pbrlib/Rendering/VulkanWrapper/CommandBuffer.hpp>

#include <cassert>

namespace pbrlib
{
    DeviceQueue::DeviceQueue(
        const Device*   ptr_device, 
        uint32_t        family_index, 
        uint32_t        index
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
        _ptr_device     (device_queue._ptr_device),
        _queue_handle   (VK_NULL_HANDLE),
        _family_index   (device_queue._family_index),
        _index          (device_queue._index)
    {
        std::swap(_queue_handle, device_queue._queue_handle);
    }

    DeviceQueue::~DeviceQueue()
    {
        /// Деструктор должен завершиться только после того,
        /// как все команды в данной очереди будут выполнены.
        vkQueueWaitIdle(_queue_handle);
    }

   void DeviceQueue::submit(const CommandBuffer& command_buffer) const
   {
        VkSubmitInfo submit_info = { };
        submit_info.sType               = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount  = 1;
        submit_info.pCommandBuffers     = &command_buffer.getCommandBufferHandle();

        assert(vkQueueSubmit(
           _queue_handle,
           1,
           &submit_info,
           VK_NULL_HANDLE
        ) == VK_SUCCESS);
   }

    const Device* DeviceQueue::getDevice() const noexcept
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
        std::shared_ptr<const Swapchain>    ptr_swapchain,
        uint32_t                            image_index
    )
    {
        VkPresentInfoKHR present_info = { };
        present_info.sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.swapchainCount = 1;
        present_info.pSwapchains    = &ptr_swapchain->getSwapchainHandle();
        present_info.pImageIndices  = &image_index;

        assert(vkQueuePresentKHR(_queue_handle, &present_info) == VK_SUCCESS);
    }

    void DeviceQueue::setPresent(
        const Swapchain&    swapchain,
        uint32_t            image_index
    )
    {
        VkPresentInfoKHR present_info = { };
        present_info.sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.swapchainCount = 1;
        present_info.pSwapchains    = &swapchain.getSwapchainHandle();
        present_info.pImageIndices  = &image_index;

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

    std::unique_ptr<DeviceQueue> DeviceQueue::make(const Device* ptr_device, uint32_t family_index, uint32_t index)
    {
        return std::make_unique<DeviceQueue>(ptr_device, family_index, index);
    }
}
