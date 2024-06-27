//
//  PhysicalDevice.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <numeric>

#include <cassert>
#include <stdexcept>

namespace pbrlib
{
    PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device_handle) :
        physical_device_handle  (physical_device_handle),
        memory                  (physical_device_handle)
    {
        uint32_t num = 0;

        assert(vkEnumerateDeviceExtensionProperties(physical_device_handle, nullptr, &num, nullptr) == VK_SUCCESS);

        std::vector<VkExtensionProperties> extension_properties (num);

        assert(vkEnumerateDeviceExtensionProperties(physical_device_handle, nullptr, &num, extension_properties.data()) == VK_SUCCESS);

        for (size_t i{0}; i < extension_properties.size(); i++)
            _search_extension_names.insert(extension_properties[i].extensionName);

        assert(vkEnumerateDeviceLayerProperties(physical_device_handle, &num, nullptr) == VK_SUCCESS);

        std::vector<VkLayerProperties> layer_properties (num);

        assert(vkEnumerateDeviceLayerProperties(physical_device_handle, &num, layer_properties.data()) == VK_SUCCESS);

        for (size_t i{0}; i < layer_properties.size(); i++)
            _search_layer_names.insert(layer_properties[i].layerName);
    }

    bool PhysicalDevice::isExtensionSupported(const std::string& name) const
    {
        return _search_extension_names.find(name) != _search_extension_names.end();
    }

    bool PhysicalDevice::isLayerSupported(const std::string& name) const
    {
        return _search_layer_names.find(name) != _search_layer_names.end();
    }

    bool PhysicalDevice::isFormatSupported(
        VkFormat            format, 
        VkImageType         image_type, 
        VkImageTiling       image_tiling, 
        VkImageUsageFlags   image_usage
    ) const
    {
        VkImageFormatProperties image_format_properties;

        return vkGetPhysicalDeviceImageFormatProperties(
            physical_device_handle, 
            format, 
            image_type, 
            image_tiling, 
            image_usage, 
            0, 
            &image_format_properties
        ) != VK_ERROR_FORMAT_NOT_SUPPORTED;
    }

    VkFormatProperties PhysicalDevice::getFormatProperties(VkFormat format) const
    {   
        VkFormatProperties format_properties;

        vkGetPhysicalDeviceFormatProperties(physical_device_handle, format, &format_properties);

        return format_properties;
    }

    std::unique_ptr<Device> PhysicalDevice::makeDevice(const Instance* ptr_instance, std::span<const VkDeviceQueueCreateInfo> queue_info) const
    {
        return std::make_unique<Device>(ptr_instance, *this, queue_info);
    }

    std::unique_ptr<Device> PhysicalDevice::makeDevice(
        const Instance*                             ptr_instance,
        std::span<const VkDeviceQueueCreateInfo>    queue_info,
        std::span<const char*>                      layer_names,
        std::span<const char*>                      extension_names
    ) const
    {
        return std::make_unique<Device>(ptr_instance, *this, queue_info, layer_names, extension_names);
    }

    uint32_t PhysicalDevice::getMemoryTypeIndex(uint32_t type) const
    {
        constexpr uint32_t invalid_type_index = std::numeric_limits<uint32_t>::max();

        VkPhysicalDeviceMemoryProperties memory_properties;

        vkGetPhysicalDeviceMemoryProperties(physical_device_handle, &memory_properties);

        for (uint32_t i{0}; i < memory_properties.memoryTypeCount; i++) {
            if ((memory_properties.memoryTypes[i].propertyFlags & type) == type) {
                return i;
            }
        }

        return invalid_type_index;
    }

    std::vector<uint32_t> PhysicalDevice::getAllMemoryTypeIndicies(uint32_t type) const
    {
        std::vector<uint32_t> memory_type_indicies;
        VkPhysicalDeviceMemoryProperties memory_properties;

        vkGetPhysicalDeviceMemoryProperties(physical_device_handle, &memory_properties);

        for (uint32_t i{0}; i < memory_properties.memoryTypeCount; i++) 
        {
            if ((memory_properties.memoryTypes[i].propertyFlags & type) == type)
                memory_type_indicies.push_back(i);
        }

        return memory_type_indicies;
    }

    std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const
    {
        uint32_t num_queue_families = 0;
        
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_handle, &num_queue_families, nullptr);
        assert(num_queue_families);

        std::vector<VkQueueFamilyProperties> properties (num_queue_families);

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_handle, &num_queue_families, properties.data());

        return properties;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PhysicalDevice::Memory::Memory(VkPhysicalDevice physical_device_handle) 
    {
        vkGetPhysicalDeviceMemoryProperties(physical_device_handle, &_memory_properties);
    }

    uint32_t PhysicalDevice::Memory::getMemoryType(VkMemoryPropertyFlags property_flags) const
    {
        for (uint32_t i{0}; i < _memory_properties.memoryTypeCount; i++) {
            if (_memory_properties.memoryTypes[i].propertyFlags & property_flags) {
                return i;
            }
        }

        throw std::invalid_argument ("Типа памяти с таким флагом нет.");
    }
}
