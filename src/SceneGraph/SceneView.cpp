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

#include "../Rendering/VulkanWrapper/Swapchain.hpp"

#include "../Rendering/VulkanWrapper/DeviceQueue.hpp"

#define PBRLIB_DEBUG_VULKAN 0

namespace pbrlib
{
    PtrInstance getVulkanInstance()
    {
        static PtrInstance ptr_instance = nullptr;

        if (!ptr_instance) {
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
            ptr_instance = Instance::make("PBRLib", VK_MAKE_VERSION(0, 0, 1), pointers_on_layer_name, pointers_on_extension_name);
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
        const PtrInstance&                      instance,
        const vector<VkDeviceQueueCreateInfo>&  queues_infos,
        const PtrPhysicalDevice&                ptr_physical_device
    )
    {
        static PtrDevice ptr_device = nullptr;

        if (!ptr_device) {
            vector<const char*> pointers_layers_names       (0);
            vector<const char*> pointers_extencions_names   (1);

            pointers_extencions_names[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

            ptr_device = ptr_physical_device->makeDevice(
                instance,
                queues_infos, 
                pointers_layers_names, 
                pointers_extencions_names
            );
        }

        return ptr_device;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SceneView::VulkanResources::VulkanResources()
    {
        ptr_instance        = getVulkanInstance();
        ptr_physical_device = getVulkanPhysicalDevice(ptr_instance);

        auto device_queues_create_info = getVulkanDeviceQueuesCreateInfo(ptr_physical_device);

        ptr_device          = getVulkanDevice(ptr_instance, device_queues_create_info, ptr_physical_device);
        ptr_device_queue    = DeviceQueue::make(ptr_device, device_queues_create_info[0].queueFamilyIndex, 0); //!< На данный момент используется только одна очередь.
        ptr_command_pool    = CommandPool::make(ptr_device, ptr_device_queue->getFamilyIndex());
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    SceneView::VulkanResources SceneView::_vulkan_resources = SceneView::VulkanResources();

    SceneView::SceneView(const string_view scene_name, const PtrWindow& ptr_window) :
        _ptr_renderer   (nullptr),
        _ptr_window     (ptr_window),
        _scene          (
            scene_name, 
            _vulkan_resources.ptr_device, 
            _vulkan_resources.ptr_device_queue,
            _vulkan_resources.ptr_command_pool,
            _vulkan_resources.ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
            _vulkan_resources.ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        )
    {
        if (!_ptr_window->_hasVulkanResources()) {
            _ptr_window->_initVulkanResources(
                _vulkan_resources.ptr_instance, 
                _vulkan_resources.ptr_physical_device, 
                _vulkan_resources.ptr_device, 
                _vulkan_resources.ptr_device->getDeviceQueueInfo()[0].queueFamilyIndex
            );
        }
    }

    Scene& SceneView::getScene() noexcept
    {
        return _scene;
    }

    const Scene& SceneView::getScene() const noexcept
    {
        return _scene;
    }

    PtrWindow& SceneView::getWindow() noexcept
    {
        return _ptr_window;
    }

    const PtrWindow& SceneView::getWindow() const noexcept
    {
        return _ptr_window;
    }

    void SceneView::setRenderer(const PtrIRenderer& ptr_renderer)
    {
        _ptr_renderer = ptr_renderer;
        _ptr_renderer->init(_ptr_window, _vulkan_resources.ptr_device, _vulkan_resources.ptr_physical_device);
    }

    void SceneView::drawScene(float delta_time)
    {
        assert(_ptr_renderer);

        if (_scene._ptr_camera_node) {
            _scene.update(delta_time);

            _ptr_renderer->draw(
                _scene._ptr_camera_node,
                _scene.getVisibleList(),
                _scene._point_light_nodes,
                _scene._spot_light_nodes,
                _scene._dir_light_nodes,
                delta_time
            );
        }
    }
}
