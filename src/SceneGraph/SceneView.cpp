//
//  SceneView.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "SceneView.hpp"

#include "../Rendering/Renderers/IRenderer.h"

#include "../Rendering/Window.hpp"
#include "../Rendering/VulkanWrapper/PhysicalDevice.hpp"

namespace pbrlib
{
    PtrInstance& getVulkanInstance(const Window& window)
    {
        static PtrInstance ptr_instance = nullptr;

        if (!ptr_instance) {
            vector<string>      extensions = Instance::getExtensionNames();
            vector<const char*> pointers_on_extension_name (extensions.size());

            vector<const char*> layers (0);

            for (size_t i{0}; i < extensions.size(); i++) {
                pointers_on_extension_name[i] = extensions[i].data();
            }

            Window::getVulkanInstanceExtensions(window, pointers_on_extension_name);

            ptr_instance = Instance::make(window.getTitle(), VK_MAKE_VERSION(0, 0, 1), layers, pointers_on_extension_name);
        }

        return ptr_instance;
    }

    PtrPhysicalDevice& getVulkanPhysicalDevice(const PtrInstance& ptr_instance)
    {
        static PtrPhysicalDevice ptr_physical_device = nullptr;

        if (!ptr_physical_device) {
            ptr_physical_device = ptr_instance->getPhysicalDevice(
                VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU    |
                VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU  |
                VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU   
            );
        }

        return ptr_physical_device;
    }

    vector<VkDeviceQueueCreateInfo> getVulkanDeviceQueuesCreateInfo(const PtrPhysicalDevice& ptr_physical_device)
    {
        static vector<VkDeviceQueueCreateInfo>  queues_infos; 
        static float                            priority = 1.0f;

        if (queues_infos.empty()) {
            vector<VkQueueFamilyProperties> queue_family_properties = ptr_physical_device->getQueueFamilyProperties();

             for (size_t i{0}; i < queue_family_properties.size(); i++) {
                if (
                    (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && 
                    (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
                ) {
                    /// Пока что будет поддерживаться только одна очередь и
                    /// только одно семейство очередей.
                    queues_infos.push_back({
                        .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                        .queueFamilyIndex   = static_cast<uint32_t>(i),
                        .queueCount         = 1,
                        .pQueuePriorities   = &priority
                    });

                    break;
                }
            }
        }

        return queues_infos;
    }

    PtrDevice& getVulkanDevice(
        const PtrInstance&                  instance,
        const vector<VkDeviceQueueCreateInfo>&    queues_infos,
        const PtrPhysicalDevice&                  ptr_physical_device
    )
    {
        static PtrDevice ptr_device  = nullptr;

        if (!ptr_device) {
            vector<const char*> pointers_layers_names       (0);
            vector<const char*> pointers_extencions_names   (1);

            pointers_extencions_names[0]    = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
            ptr_device                      = ptr_physical_device->makeDevice(
                queues_infos, 
                pointers_layers_names, 
                pointers_extencions_names
            );
        }

        return ptr_device;
    }

    SceneView::SceneView(const string_view scene_name, const Window& window) :
        _ptr_instance       (getVulkanInstance(window)),
        _ptr_physical_device(getVulkanPhysicalDevice(_ptr_instance)),
        _queues_infos       (getVulkanDeviceQueuesCreateInfo(_ptr_physical_device)),
        _ptr_device         (
            getVulkanDevice(
                getVulkanInstance(window), 
                getVulkanDeviceQueuesCreateInfo(
                    getVulkanPhysicalDevice(getVulkanInstance(window))
                ), 
                getVulkanPhysicalDevice(getVulkanInstance(window))
            )
        ),
        _ptr_renderer   (nullptr),
        _scene          (
            scene_name, 
            _ptr_device, 
            _ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
            _queues_infos[0].queueFamilyIndex
        )
    {}

    void SceneView::setRenderer(const PtrIRenderer& ptr_renderer)
    {
        _ptr_renderer = ptr_renderer;
    }

    PtrInstance& SceneView::getInstance() noexcept
    {
        return _ptr_instance;
    }

    const PtrInstance& SceneView::getInstance() const noexcept
    {
        return _ptr_instance;
    }

    PtrDevice& SceneView::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& SceneView::getDevice() const noexcept
    {
        return _ptr_device;
    }

    PtrIRenderer& SceneView::getRenderer() noexcept
    {
        return _ptr_renderer;
    }

    const PtrIRenderer& SceneView::getRenderer() const noexcept
    {
        return _ptr_renderer;
    }

    Scene& SceneView::getScene() noexcept
    {
        return _scene;
    }

    const Scene& SceneView::getScene() const noexcept
    {
        return _scene;
    }

    void SceneView::drawScene(Window& window, float delta_time)
    {
        /**
         * TODO: Дописать этот метод.
        */
    }
}
