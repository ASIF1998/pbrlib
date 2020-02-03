//
//  Device.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Device.hpp"

#include <stdexcept>

namespace pbrlib
{
    Device::Device(PhysicalDevice physical_device, const vector<VkDeviceQueueCreateInfo>& queue_info) :
        _device_handle(VK_NULL_HANDLE),
        _physical_device(physical_device)
    {
        vector<VkQueueFamilyProperties> queue_properties (queue_info.size());
        uint32_t queue_num = static_cast<uint32_t>(queue_properties.size());

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device.physical_device_handle, &queue_num, queue_properties.data());

        for (size_t i{0}; i < queue_properties.size(); i++) {
            if (!(queue_properties[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT))) {
                throw invalid_argument("Семейство очередей #" + to_string(i) + " не поддерживает графические операции.");
            }
        }

        VkDeviceCreateInfo device_info {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<uint32_t>(queue_info.size()),
            .pQueueCreateInfos = queue_info.data(),
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr,
            .pEnabledFeatures = nullptr
        };

        assert(vkCreateDevice(physical_device.physical_device_handle, &device_info, nullptr, &_device_handle) == VK_SUCCESS);
        assert(_device_handle != VK_NULL_HANDLE);
    }

    Device::Device(PhysicalDevice physical_device, 
                   const vector<VkDeviceQueueCreateInfo>& queue_info, 
                   const vector<const char*>& layer_names, 
                   const vector<const char*>& extension_names) :
        _device_handle(VK_NULL_HANDLE),
        _physical_device(physical_device)
    {
        vector<VkQueueFamilyProperties> queue_properties (queue_info.size());
        uint32_t queue_num = static_cast<uint32_t>(queue_properties.size());

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device.physical_device_handle, &queue_num, queue_properties.data());

        for (size_t i{0}; i < queue_properties.size(); i++) {
            if (!(queue_properties[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT))) {
                throw invalid_argument("Семейство очередей #" + to_string(i) + " не поддерживает графические операции.");
            }
        }

        VkDeviceCreateInfo device_info {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<uint32_t>(queue_info.size()),
            .pQueueCreateInfos = queue_info.data(),
            .enabledLayerCount = static_cast<uint32_t>(layer_names.size()),
            .ppEnabledLayerNames = layer_names.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extension_names.size()),
            .ppEnabledExtensionNames = extension_names.data(),
            .pEnabledFeatures = nullptr
        };

        assert(vkCreateDevice(physical_device.physical_device_handle, &device_info, nullptr, &_device_handle) == VK_SUCCESS);
        assert(_device_handle != VK_NULL_HANDLE);
    }

    Device::~Device()
    {
        vkDestroyDevice(_device_handle, nullptr);
    }

    VkDevice Device::getHandle() const noexcept
    {
        return _device_handle;
    }

    PhysicalDevice& Device::getPhysicalDevice() noexcept
    {
        return _physical_device;
    }

    const PhysicalDevice& Device::getPhysicalDevice() const noexcept
    {
        return _physical_device;
    }

    // VkQueue Device::getQueue(uint32_t famili_index, uint32_t index) const
    // {
    //     VkQueue queue = VK_NULL_HANDLE;

    //     vkGetDeviceQueue(_device_handle, famili_index, index, &queue);

    //     return queue;
    // }

    // VkQueue Device::getQueueByPresent() const
    // {
    //     return _present_queue_handle;
    // }

    void Device::waitIdle() const
    {
        vkDeviceWaitIdle(_device_handle);
    }
}
