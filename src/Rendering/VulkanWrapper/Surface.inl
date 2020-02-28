//
//  Surface.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline Surface::Surface(
        const Window&               window, 
        const shared_ptr<Instance>& ptr_instance, 
        const PhysicalDevice&       physical_device
    ) :
        _ptr_instance   (ptr_instance),
        _surface_handle (VK_NULL_HANDLE)
    {
        assert(SDL_Vulkan_CreateSurface(window._ptr_window, _ptr_instance->getHandle(), &_surface_handle) == SDL_TRUE);
        assert(_surface_handle);
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device.physical_device_handle, _surface_handle, &_surface_capabilities);
    }

    inline Surface::Surface(Surface&& surface) :
        _ptr_instance           (surface._ptr_instance),
        _surface_handle         (VK_NULL_HANDLE),
        _surface_capabilities   (surface._surface_capabilities),
        _surface_format         (surface._surface_format)
    {
        swap(_surface_handle, surface._surface_handle);
    }

    inline Surface::~Surface()
    {
        if (_surface_handle != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(_ptr_instance->getHandle(), _surface_handle, nullptr);
        }
    }

    inline shared_ptr<Surface> Surface::make(
        const Window&               window,
        const shared_ptr<Instance>& ptr_instance,
        const PhysicalDevice&       physical_device
    )
    {
        return make_shared<Surface>(window, ptr_instance, physical_device);
    }

    inline const VkSurfaceKHR& Surface::getSurfaceHandle() const noexcept
    {
        return _surface_handle;
    }

    inline shared_ptr<Instance>& Surface::getInstance() noexcept
    {
        return _ptr_instance;
    }

    inline const shared_ptr<Instance>& Surface::getInstance() const noexcept
    {
        return _ptr_instance;
    }

    inline const VkSurfaceCapabilitiesKHR& Surface::getSurfaceCapabilities() const noexcept
    {
        return _surface_capabilities;
    }
        
    inline const VkSurfaceFormatKHR& Surface::getSurfaceFormat() const noexcept
    {
        return _surface_format;
    }

    inline void Surface::setFormat(const VkSurfaceFormatKHR& format) noexcept
    {
        _surface_format = format;
    }

    inline vector<VkSurfaceFormatKHR> Surface::getAllSurfaceFormats(
        const Surface&          surface, 
        const PhysicalDevice&   physical_device
    )
    {
        uint32_t num_formats = 0;

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            physical_device.physical_device_handle, 
            surface._surface_handle, 
            &num_formats, 
            nullptr
        );

        vector<VkSurfaceFormatKHR> formats (num_formats);

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            physical_device.physical_device_handle, 
            surface._surface_handle, 
            &num_formats, 
            formats.data()
        );

        return formats;
    }
}
