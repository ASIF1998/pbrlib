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

#include "../PBRLibResources.hpp"

namespace pbrlib
{
    SceneView::SceneView(const string_view scene_name, const PtrPBRLibResources& ptr_pbrlib_resources, const PtrWindow& ptr_window) :
        _ptr_pbrlib_resources   (ptr_pbrlib_resources),
        _ptr_renderer           (nullptr),
        _ptr_window             (ptr_window),
        _scene                  (
            scene_name,
            ptr_pbrlib_resources->_vk_resources._ptr_device,
            ptr_pbrlib_resources->_vk_resources._ptr_device_queue,
            ptr_pbrlib_resources->_vk_resources._ptr_command_pool,
            ptr_pbrlib_resources->_vk_resources._ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
            ptr_pbrlib_resources->_vk_resources._ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        )
    {
        if (!_ptr_window->_hasVulkanResources()) {
            _ptr_window->_initVulkanResources(
                _ptr_pbrlib_resources->_vk_resources._ptr_instance,
                _ptr_pbrlib_resources->_vk_resources._ptr_physical_device,
                _ptr_pbrlib_resources->_vk_resources._ptr_device,
                _ptr_pbrlib_resources->_vk_resources._ptr_device->getDeviceQueueInfo()[0].queueFamilyIndex
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
        _ptr_renderer->init(
            _ptr_window, 
            _ptr_pbrlib_resources->_vk_resources._ptr_device, 
            _ptr_pbrlib_resources->_vk_resources._ptr_physical_device
        );
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
