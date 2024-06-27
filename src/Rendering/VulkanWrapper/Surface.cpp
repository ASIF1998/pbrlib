//
//  Surface.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/Surface.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Instance.hpp>
#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <cassert>

namespace pbrlib
{
    Surface::Surface(
        const Window&           window, 
        const Instance*         ptr_instance, 
        const PhysicalDevice*   ptr_physical_device
    ) :
        _ptr_instance   (ptr_instance),
        _surface_handle (VK_NULL_HANDLE)
    {
        assert(SDL_Vulkan_CreateSurface(window._ptr_window, _ptr_instance->getHandle(), nullptr, &_surface_handle) == SDL_TRUE);
        assert(_surface_handle);
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ptr_physical_device->physical_device_handle, _surface_handle, &_surface_capabilities);
    }

    Surface::Surface(Surface&& surface) :
        _ptr_instance           (std::move(surface._ptr_instance)),
        _surface_handle         (VK_NULL_HANDLE),
        _surface_capabilities   (surface._surface_capabilities),
        _surface_format         (surface._surface_format)
    {
        std::swap(_surface_handle, surface._surface_handle);
    }

    Surface::~Surface()
    {
        if (_surface_handle != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(_ptr_instance->getHandle(), _surface_handle, nullptr);
        }
    }

    std::unique_ptr<Surface> Surface::make(
        const Window&           window,
        const Instance*         ptr_instance, 
        const PhysicalDevice*   ptr_physical_device
    )
    {
        return std::make_unique<Surface>(window, ptr_instance, ptr_physical_device);
    }

    const VkSurfaceKHR& Surface::getSurfaceHandle() const noexcept
    {
        return _surface_handle;
    }

    const Instance* Surface::getInstance() const noexcept
    {
        return _ptr_instance;
    }

    const VkSurfaceCapabilitiesKHR& Surface::getSurfaceCapabilities() const noexcept
    {
        return _surface_capabilities;
    }
        
    const VkSurfaceFormatKHR& Surface::getSurfaceFormat() const noexcept
    {
        return _surface_format;
    }

    void Surface::setFormat(const VkSurfaceFormatKHR& format) noexcept
    {
        _surface_format = format;
    }

    std::vector<VkSurfaceFormatKHR> Surface::getAllSurfaceFormats(
        const Surface&          surface, 
        const PhysicalDevice*   ptr_physical_device
    )
    {
        uint32_t num_formats = 0;

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            ptr_physical_device->physical_device_handle, 
            surface._surface_handle, 
            &num_formats, 
            nullptr
        );

        std::vector<VkSurfaceFormatKHR> formats (num_formats);

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            ptr_physical_device->physical_device_handle, 
            surface._surface_handle, 
            &num_formats, 
            formats.data()
        );

        return formats;
    }
}
