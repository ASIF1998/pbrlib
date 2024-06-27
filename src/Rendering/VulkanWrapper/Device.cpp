//
//  Device.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/Device.hpp>
#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <stdexcept>
#include <cassert>

namespace pbrlib
{
    Device::Device(
        const Instance*                             ptr_instance,
        const PhysicalDevice&                       physical_device, 
        std::span<const VkDeviceQueueCreateInfo>    queue_infos
    ) :
        _device_handle(VK_NULL_HANDLE)
    {
        _create(ptr_instance, physical_device, queue_infos, 0, nullptr, 0, nullptr);
    }

    Device::Device(
        const Instance*                             ptr_instance,
        const PhysicalDevice&                       physical_device, 
        std::span<const VkDeviceQueueCreateInfo>    queue_infos, 
        std::span<const char*>                      layer_names, 
        std::span<const char*>                      extension_names
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
        std::swap(_device_handle, device._device_handle);
    }

    Device::~Device()
    {
        if (_device_handle != VK_NULL_HANDLE)
            vkDestroyDevice(_device_handle, nullptr);
    }

    const VkDevice& Device::getDeviceHandle() const noexcept
    {
        return _device_handle;
    }

    const std::vector<VkDeviceQueueCreateInfo>& Device::getDeviceQueueInfo() const noexcept
    {
        return _queues_infos;
    }

    const Instance* Device::getInstance() const noexcept
    {
        return _ptr_instance;
    }

    void Device::waitIdle() const
    {
        vkDeviceWaitIdle(_device_handle);
    }

    void Device::_create(
        const Instance*                             ptr_instance,
        const PhysicalDevice&                       physical_device,
        std::span<const VkDeviceQueueCreateInfo>    queue_infos,
        uint32_t                                    enabled_layer_count,
        const char* const*                          ptr_enabled_layers,
        uint32_t                                    enabled_extension_count,
        const char* const*                          ptr_extensions
    )
    {
        _ptr_instance = ptr_instance;  
        _queues_infos = std::vector(std::begin(queue_infos), std::end(queue_infos));

        std::vector<VkQueueFamilyProperties> queue_properties (queue_infos.size());
        uint32_t queue_num = static_cast<uint32_t>(queue_properties.size());

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device.physical_device_handle, &queue_num, queue_properties.data());

        for (size_t i{0}; i < queue_properties.size(); i++) 
        {
            if (!(queue_properties[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT)))
                throw std::invalid_argument("Семейство очередей #" + std::to_string(i) + " не поддерживает графические операции.");
        }

        VkDeviceCreateInfo device_info = { };
        device_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_infos.size());
        device_info.pQueueCreateInfos       = queue_infos.data();
        device_info.enabledLayerCount       = enabled_layer_count;
        device_info.ppEnabledLayerNames     = ptr_enabled_layers;
        device_info.enabledExtensionCount   = enabled_extension_count;
        device_info.ppEnabledExtensionNames = ptr_extensions;

        assert(vkCreateDevice(
            physical_device.physical_device_handle, 
            &device_info, 
            nullptr, 
            &_device_handle
        ) == VK_SUCCESS);
        
        assert(_device_handle != VK_NULL_HANDLE);
    }
}
