//
//  Instance.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Instance.hpp"
#include "PhysicalDevice.hpp"

#include "../../core.hpp"

namespace pbrlib
{
    VulkanInstanceExtensionSupported Instance::_supported_extensions = {};
    VulkanInstanceLayerSupported Instance::_supported_layers = {};

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

    bool VulkanInstanceExtensionSupported::check(const string& name) const
    {
        return _extension_supported.find(name) != _extension_supported.end();
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

    bool VulkanInstanceLayerSupported::check(const string& name) const
    {
        return _layer_supported.find(name) != _layer_supported.end();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Instance::Instance(const string_view application_name, uint32_t app_version) :
        _instance_handle(VK_NULL_HANDLE)
    {
        VkApplicationInfo app_info {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = application_name.data(),
            .applicationVersion = app_version,
            .pEngineName = "PBRLib",
            .engineVersion = EngineVersion,
            .apiVersion = VK_MAKE_VERSION(1, 0, 0)
        };

        VkInstanceCreateInfo instance_info {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr
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

    Instance::Instance(const string_view application_name, 
                       uint32_t app_version, 
                       const vector<const char*>& layer_names, 
                       const vector<const char*>& extension_names) :
        _instance_handle(VK_NULL_HANDLE)
    {
        VkApplicationInfo app_info {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = application_name.data(),
            .applicationVersion = app_version,
            .pEngineName = "PBRLib",
            .engineVersion = EngineVersion,
            .apiVersion = VK_MAKE_VERSION(1, 0, 0)
        };

        VkInstanceCreateInfo instance_info {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<uint32_t>(layer_names.size()),
            .ppEnabledLayerNames = layer_names.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extension_names.size()),
            .ppEnabledExtensionNames = extension_names.data()
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

    Instance::~Instance()
    {
        vkDestroyInstance(_instance_handle, nullptr);
    }

    VkInstance Instance::getHandle() const
    {
        return _instance_handle;
    }

    PhysicalDevice Instance::getGPUHandle(int type) const
    {
        VkPhysicalDeviceProperties physical_device_property;

        for (size_t i{0}; i < _physical_device_handles.size(); i++) {
            vkGetPhysicalDeviceProperties(_physical_device_handles[i].physical_device_handle, &physical_device_property);
            if (physical_device_property.deviceType & type) {
                return _physical_device_handles[i];
            }
        }

        return VK_NULL_HANDLE;
    }

    vector<PhysicalDevice> Instance::getAllGPUHandle(int type) const
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

    bool Instance::isExtensionSupported(const string& name)
    {
        return _supported_extensions.check(name);
    }

    bool Instance::isLayerSupported(const string& name)
    {
        return _supported_layers.check(name);
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
