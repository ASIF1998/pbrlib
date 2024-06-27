//
//  SceneView.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/SceneGraph/SceneView.hpp>

#include <pbrlib/Rendering/Renderers/IRenderer.h>

#include <pbrlib/Rendering/Window.hpp>

#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Swapchain.hpp>
#include <pbrlib/Rendering/VulkanWrapper/DeviceQueue.hpp>

#include <pbrlib/PBRLibResources.hpp>

namespace pbrlib
{
    SceneView::SceneView(const std::string_view scene_name, std::shared_ptr<const PBRLibResources> ptr_pbrlib_resources, std::shared_ptr<Window> ptr_window) :
        _ptr_pbrlib_resources   (ptr_pbrlib_resources),
        _ptr_renderer           (nullptr),
        _ptr_window             (ptr_window),
        _scene                  (
            scene_name,
            ptr_pbrlib_resources->_vk_resources._ptr_device.get(),
            ptr_pbrlib_resources->_vk_resources._ptr_device_queue.get(),
            ptr_pbrlib_resources->_vk_resources._ptr_command_pool.get(),
            ptr_pbrlib_resources->_vk_resources._ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
            ptr_pbrlib_resources->_vk_resources._ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        )
    {
        if (!_ptr_window->_hasVulkanResources()) {
            _ptr_window->_initVulkanResources(
                _ptr_pbrlib_resources->_vk_resources._ptr_instance.get(),
                _ptr_pbrlib_resources->_vk_resources._ptr_physical_device.get(),
                _ptr_pbrlib_resources->_vk_resources._ptr_device.get(),
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

    std::shared_ptr<const Window> SceneView::getWindow() const noexcept
    {
        return _ptr_window;
    }

    void SceneView::setRenderer(std::shared_ptr<IRenderer> ptr_renderer)
    {
        _ptr_renderer = ptr_renderer;
        _ptr_renderer->init(
            _ptr_window, 
            _ptr_pbrlib_resources->_vk_resources._ptr_device.get(), 
            _ptr_pbrlib_resources->_vk_resources._ptr_physical_device.get()
        );
    }

    void SceneView::drawScene(float delta_time)
    {
        assert(_ptr_renderer);

        if (_scene._ptr_camera)
        {
            _scene.update(delta_time);

            _ptr_renderer->draw(
                _scene._ptr_camera,
                _scene.getVisibleList(),
                std::span(_scene._point_lights),
                _scene._spot_lights,
                _scene._dir_lights,
                delta_time
            );
        }
    }
}
