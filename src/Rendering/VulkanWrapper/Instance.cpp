//
//  Instance.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/Instance.hpp>
#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <pbrlib/core.hpp>

#include <cassert>
#include <stdexcept>

using namespace std;

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
    Instance::Instance(const string_view app_name, uint32_t app_version) :
        _instance_handle(VK_NULL_HANDLE)
    {
        _create(app_name, app_version, 0, nullptr, 0, nullptr);
    }

    Instance::Instance(
        const string_view           app_name, 
        uint32_t                    app_version, 
        span<const char*>           layer_names, 
        span<const char*>           extension_names
    ) :
        _instance_handle(VK_NULL_HANDLE)
    {
        _create(
            app_name,
            app_version,
            static_cast<uint32_t>(layer_names.size()),
            layer_names.data(),
            static_cast<uint32_t>(extension_names.size()),
            extension_names.data()
        );
    }

    Instance::Instance(Instance&& instance) :
        _instance_handle        (VK_NULL_HANDLE),
        _physical_device_handles(move(instance._physical_device_handles))
    {
        swap(_instance_handle, instance._instance_handle);
    }

    Instance::~Instance()
    {
        if (_instance_handle != VK_NULL_HANDLE) {
            vkDestroyInstance(_instance_handle, nullptr);
        }
    }

    void Instance::_create(
        const string_view   app_name,
        uint32_t            app_version, 
        uint32_t            enabled_layer_count,
        const char* const*  ptr_enable_layers,
        uint32_t            enabled_extension_count,
        const char* const*  ptr_extensions
    )
    {
        VkApplicationInfo app_info = { };
        app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName   = app_name.data();
        app_info.apiVersion         = app_version;
        app_info.pEngineName        = "PBRLib";
        app_info.engineVersion      = EngineVersion;
        app_info.apiVersion         = VulkanVersion;

        VkInstanceCreateInfo instance_info = { };
        instance_info.sType                     = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_info.pApplicationInfo          = &app_info;
        instance_info.enabledLayerCount         = enabled_layer_count;
        instance_info.ppEnabledLayerNames       = ptr_enable_layers;
        instance_info.enabledExtensionCount     = enabled_extension_count;
        instance_info.ppEnabledExtensionNames   = ptr_extensions;

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

    const VkInstance& Instance::getHandle() const
    {
        return _instance_handle;
    }

    bool Instance::isExtensionSupported(const string& name)
    {
        return _supported_extensions.check(name);
    }

    bool Instance::isLayerSupported(const string& name)
    {
        return _supported_layers.check(name);
    }

    PtrInstance Instance::make(const string_view app_name, uint32_t app_version)
    {
        return make_shared<Instance>(app_name, app_version);
    }

    PtrInstance Instance::make(
        const string_view           app_name,
        uint32_t                    app_version,
        span<const char*>           layer_names,
        span<const char*>           extension_names
    )
    {
        return make_shared<Instance>(app_name, app_version, layer_names, extension_names);
    }

    PtrPhysicalDevice Instance::getPhysicalDevice(int type)
    {
        VkPhysicalDeviceProperties physical_device_property;

        for (size_t i{0}; i < _physical_device_handles.size(); i++) {
            vkGetPhysicalDeviceProperties(_physical_device_handles[i].physical_device_handle, &physical_device_property);
            if (physical_device_property.deviceType & type) {
                return make_shared<PhysicalDevice>(_physical_device_handles[i]);
            }
        }

        throw invalid_argument ("Такого типа устройства нет.");
    }

    vector<PtrPhysicalDevice> Instance::getAllPhysicalDevice(int type) const
    {
        vector<PtrPhysicalDevice> handles;
        VkPhysicalDeviceProperties physical_device_property;

        for (size_t i{0}; i < _physical_device_handles.size(); i++) {
            vkGetPhysicalDeviceProperties(_physical_device_handles[i].physical_device_handle, &physical_device_property);
            if (physical_device_property.deviceType & type) {
                handles.push_back(make_shared<PhysicalDevice>(_physical_device_handles[i]));
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
