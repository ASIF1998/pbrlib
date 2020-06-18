//
//  Device.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Device.hpp"

#include <stdexcept>

#include "PhysicalDevice.hpp"

namespace pbrlib
{
    Device::Device(
        const PtrInstance&                      ptr_instance,
        const PhysicalDevice&                   physical_device, 
        const vector<VkDeviceQueueCreateInfo>&  queue_infos
    ) :
        _device_handle(VK_NULL_HANDLE)
    {
        _create(ptr_instance, physical_device, queue_infos, 0, nullptr, 0, nullptr);
    }

    Device::Device(
        const PtrInstance&                      ptr_instance,
        const PhysicalDevice&                   physical_device, 
        const vector<VkDeviceQueueCreateInfo>&  queue_infos, 
        const vector<const char*>&              layer_names, 
        const vector<const char*>&              extension_names
    ) :
        _device_handle(VK_NULL_HANDLE)
    {
        _create(
            ptr_instance,
            physical_device, queue_infos,
            static_cast<uint32_t>(layer_names.size()),
            layer_names.data(),
            static_cast<uint32_t>(extension_names.size()),
            extension_names.data()
        );
    }

    Device::Device(Device&& device) :
        _device_handle(VK_NULL_HANDLE)
    {
        swap(_device_handle, device._device_handle);
    }

    Device::~Device()
    {
        if (_device_handle != VK_NULL_HANDLE) {
            vkDestroyDevice(_device_handle, nullptr);
        }
    }

    const VkDevice& Device::getDeviceHandle() const noexcept
    {
        return _device_handle;
    }

    const vector<VkDeviceQueueCreateInfo>& Device::getDeviceQueueInfo() const noexcept
    {
        return _queues_infos;
    }

    PtrInstance& Device::getInstance() noexcept
    {
        return _ptr_instance;
    }

    const PtrInstance& Device::getInstance() const noexcept
    {
        return _ptr_instance;
    }

    void Device::waitIdle() const
    {
        vkDeviceWaitIdle(_device_handle);
    }

    void Device::_create(
        const PtrInstance&                      ptr_instance,
        const PhysicalDevice&                   physical_device,
        const vector<VkDeviceQueueCreateInfo>&  queue_infos,
        uint32_t                                enabled_layer_count,
        const char* const*                      ptr_enabled_layers,
        uint32_t                                enabled_extension_count,
        const char* const*                      ptr_extensions
    )
    {
        _ptr_instance = ptr_instance;  
        _queues_infos = queue_infos;

        vector<VkQueueFamilyProperties> queue_properties (queue_infos.size());
        uint32_t queue_num = static_cast<uint32_t>(queue_properties.size());

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device.physical_device_handle, &queue_num, queue_properties.data());

        for (size_t i{0}; i < queue_properties.size(); i++) {
            if (!(queue_properties[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT))) {
                throw invalid_argument("Семейство очередей #" + to_string(i) + " не поддерживает графические операции.");
            }
        }

        VkDeviceCreateInfo device_info {
            .sType                      = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .queueCreateInfoCount       = static_cast<uint32_t>(queue_infos.size()),
            .pQueueCreateInfos          = queue_infos.data(),
            .enabledLayerCount          = enabled_layer_count,
            .ppEnabledLayerNames        = ptr_enabled_layers,
            .enabledExtensionCount      = enabled_extension_count,
            .ppEnabledExtensionNames    = ptr_extensions,
            .pEnabledFeatures           = nullptr
        };

        assert(vkCreateDevice(
            physical_device.physical_device_handle, 
            &device_info, 
            nullptr, 
            &_device_handle
        ) == VK_SUCCESS);
        
        assert(_device_handle != VK_NULL_HANDLE);
    }
}
