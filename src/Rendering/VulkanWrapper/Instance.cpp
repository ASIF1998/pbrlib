//
//  Instance.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Instance.hpp"

#include "../../core.hpp"

namespace pbrlib
{
    VulkanInstanceExtensionSupported    Instance::_supported_extensions = {};
    VulkanInstanceLayerSupported        Instance::_supported_layers     = {};

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    VulkanInstanceExtensionSupported::VulkanInstanceExtensionSupported()
    {
        uint32_t num_extension_supported = 0;
        assert(vkEnumerateInstanceExtensionProperties(nullptr, &num_extension_supported, nullptr) == VK_SUCCESS);

        vector<VkExtensionProperties> extension_properties (num_extension_supported);

        assert(vkEnumerateInstanceExtensionProperties(nullptr, &num_extension_supported, extension_properties.data()) == VK_SUCCESS);

        for (size_t i{0}; i < extension_properties.size(); i++) {
            _extension_supported.insert(extension_properties[i].extensionName);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    VulkanInstanceLayerSupported::VulkanInstanceLayerSupported()
    {
        uint32_t num_layer_supported = 0;

        assert(vkEnumerateInstanceLayerProperties(&num_layer_supported, nullptr) == VK_SUCCESS);

        vector<VkLayerProperties> layer_properties (num_layer_supported);

        assert(vkEnumerateInstanceLayerProperties(&num_layer_supported, layer_properties.data()) == VK_SUCCESS);

        for (size_t i{0}; i < layer_properties.size(); i++) {
            _layer_supported.insert(layer_properties[i].layerName);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Instance::_create_instance(
        const string_view   app_name,
        uint32_t            app_version, 
        uint32_t            enabled_layer_count,
        const char* const*  ptr_enable_layers,
        uint32_t            enabled_extension_count,
        const char* const*  ptr_extensions
    )
    {
        VkApplicationInfo app_info {
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext              = nullptr,
            .pApplicationName   = app_name.data(),
            .applicationVersion = app_version,
            .pEngineName        = "PBRLib",
            .engineVersion      = EngineVersion,
            .apiVersion         = VK_MAKE_VERSION(1, 0, 0)
        };

        VkInstanceCreateInfo instance_info {
            .sType                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .pApplicationInfo           = &app_info,
            .enabledLayerCount          = enabled_layer_count,
            .ppEnabledLayerNames        = ptr_enable_layers,
            .enabledExtensionCount      = enabled_extension_count,
            .ppEnabledExtensionNames    = ptr_extensions
        };

        assert(vkCreateInstance(&instance_info, nullptr, &_instance_handle) == VK_SUCCESS);
        assert(_instance_handle != VK_NULL_HANDLE);

        uint32_t num_physical_device_handles = 0;

        assert(vkEnumeratePhysicalDevices(_instance_handle, &num_physical_device_handles, nullptr) == VK_SUCCESS);
        
        assert(num_physical_device_handles > 0);
        _physical_device_handles.reserve(num_physical_device_handles);
        
        vector<VkPhysicalDevice> physical_device_handles(num_physical_device_handles);
        
        assert(vkEnumeratePhysicalDevices(_instance_handle, &num_physical_device_handles, physical_device_handles.data()) == VK_SUCCESS);
        
        for (size_t i{0}; i < physical_device_handles.size(); i++) {
            _physical_device_handles.push_back(physical_device_handles[i]);
        }
    }

    PhysicalDevice& Instance::getPhysicalDevice(int type)
    {
        VkPhysicalDeviceProperties physical_device_property;

        for (size_t i{0}; i < _physical_device_handles.size(); i++) {
            vkGetPhysicalDeviceProperties(_physical_device_handles[i].physical_device_handle, &physical_device_property);
            if (physical_device_property.deviceType & type) {
                return _physical_device_handles[i];
            }
        }

        throw invalid_argument ("Такого типа устройства нет.");
    }

    vector<PhysicalDevice> Instance::getAllPhysicalDevice(int type) const
    {
        vector<PhysicalDevice> handles;
        VkPhysicalDeviceProperties physical_device_property;

        for (size_t i{0}; i < _physical_device_handles.size(); i++) {
            vkGetPhysicalDeviceProperties(_physical_device_handles[i].physical_device_handle, &physical_device_property);
            if (physical_device_property.deviceType & type) {
                handles.push_back(_physical_device_handles[i]);
            }
        }

        handles.shrink_to_fit();

        return handles;
    }

    vector<string> Instance::getExtensionNames()
    {
        uint32_t num_extension_supported = 0;
        assert(vkEnumerateInstanceExtensionProperties(nullptr, &num_extension_supported, nullptr) == VK_SUCCESS);

        vector<VkExtensionProperties> extension_properties (num_extension_supported);

        assert(vkEnumerateInstanceExtensionProperties(nullptr, &num_extension_supported, extension_properties.data()) == VK_SUCCESS);

        vector<string> extension_names (num_extension_supported);

        for (size_t i{0}; i < extension_properties.size(); i++) {
            extension_names[i] = extension_properties[i].extensionName;
        }

        return extension_names;
    }

    vector<string> Instance::getLayerNames()
    {
        uint32_t num_layer_supported = 0;

        assert(vkEnumerateInstanceLayerProperties(&num_layer_supported, nullptr) == VK_SUCCESS);

        vector<VkLayerProperties> layer_properties (num_layer_supported);

        assert(vkEnumerateInstanceLayerProperties(&num_layer_supported, layer_properties.data()) == VK_SUCCESS);

        vector<string> layer_names (num_layer_supported);

        for (size_t i{0}; i < layer_properties.size(); i++) {
            layer_names[i] = layer_properties[i].layerName;
        }

        return layer_names;
    }
}
