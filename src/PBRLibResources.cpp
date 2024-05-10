//
//  PBRLibResources.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 16.09.2021.
//

#include <pbrlib/PBRLibResources.hpp>

#include <vector>
#include <cassert>

#define PBRLIB_DEBUG_VULKAN  0

#ifndef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#   define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#endif

namespace pbrlib
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline PtrInstance createVulkanInstance()
    {
        vector<string>      extensions = Instance::getExtensionNames();
        vector<const char*> pointers_on_extension_name(extensions.size());

        vector<const char*> pointers_on_layer_name;

        for (size_t i{0}, num_ext{extensions.size()}; i < num_ext; i++) {
            pointers_on_extension_name[i] = extensions[i].data();
        }

#if PBRLIB_DEBUG_VULKAN
        vector<string> layers = Instance::getLayerNames();

        pointers_on_layer_name.resize(layers.size());

        for (size_t i{0}, num_layers{layers.size()}; i < num_layers; i++) {
            pointers_on_layer_name[i] = layers[i].data();
        }
#endif

        return Instance::make("PBRLib", VK_MAKE_VERSION(0, 0, 1), pointers_on_layer_name, pointers_on_extension_name);
    }

    inline vector<VkDeviceQueueCreateInfo> getVulkanDeviceQueuesCreateInfo(const PtrPhysicalDevice& ptr_physical_device)
    {
        vector<VkDeviceQueueCreateInfo> queues_infos; 
        static const float              priority = 1.0f;

        vector<VkQueueFamilyProperties> queue_family_properties = ptr_physical_device->getQueueFamilyProperties();

        for (size_t i{0}; i < queue_family_properties.size(); i++) {
            if (
                (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && 
                (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            ) {
                VkDeviceQueueCreateInfo queue_ci = { };
                queue_ci.sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queue_ci.pQueuePriorities   = &priority;
                queue_ci.queueCount         = 1;
                queue_ci.queueFamilyIndex   = static_cast<uint32_t>(i);

                /// Пока что будет поддерживаться только одна очередь и
                /// только одно семейство очередей.
                queues_infos.push_back(queue_ci);

                break;
            }
        }

        return queues_infos;
    }

    inline PtrDevice getVulkanDevice(
        const PtrInstance&                      instance,
        const vector<VkDeviceQueueCreateInfo>&  queues_infos,
        const PtrPhysicalDevice&                ptr_physical_device
    )
    {
        vector<const char*> pointers_layers_names       (0);
        vector<const char*> pointers_extencions_names   (1);

        pointers_extencions_names[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        
        uint32_t                        num_extensions = 0;
        vector<VkExtensionProperties>   extensions_properties;
        
        assert(vkEnumerateDeviceExtensionProperties(
            ptr_physical_device->physical_device_handle, 
            nullptr, 
            &num_extensions, nullptr
        ) == VK_SUCCESS);
        
        extensions_properties.resize(num_extensions);

        assert(vkEnumerateDeviceExtensionProperties(
            ptr_physical_device->physical_device_handle, 
            nullptr, 
            &num_extensions, extensions_properties.data()
        ) == VK_SUCCESS);
        
        for (const auto& ext_propertis: extensions_properties) {
            if (!strcmp(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, ext_propertis.extensionName)) {
                pointers_extencions_names.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
            }
        }

        return ptr_physical_device->makeDevice(
            instance,
            queues_infos,
            pointers_layers_names,
            pointers_extencions_names
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    VulkanResources::VulkanResources() :
        _ptr_instance (createVulkanInstance())
    {
        _ptr_physical_device = _ptr_instance->getPhysicalDevice(
            VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU    |
            VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU  |
            VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU   
        );

        auto device_queues_create_info = getVulkanDeviceQueuesCreateInfo(_ptr_physical_device);

        _ptr_device         = getVulkanDevice(_ptr_instance, device_queues_create_info, _ptr_physical_device);
        _ptr_device_queue   = DeviceQueue::make(_ptr_device, device_queues_create_info[0].queueFamilyIndex, 0); //!< На данный момент используется только одна очередь.
        _ptr_command_pool   = CommandPool::make(_ptr_device, _ptr_device_queue->getFamilyIndex());
    }
}
