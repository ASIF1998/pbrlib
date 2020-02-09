//
//  Surface.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include "Instance.hpp"
#include "PhysicalDevice.hpp"

#include "../Window.hpp"

#include <memory>

#include <SDL2/SDL_vulkan.h>

using namespace std;

namespace pbrlib
{
    class Swapchain;

    class Surface
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param window окно.
         * @param ptr_instance указатель на экземпляр Vulkan'а.
         * @param physical_device физическое устройтсво.
        */
        inline Surface(const Window& window, const shared_ptr<Instance>& ptr_instance, const PhysicalDevice& physical_device);
        inline ~Surface();

        inline VkSurfaceKHR getSurfaceHandle() const noexcept;
        inline shared_ptr<Instance>& getInstance() noexcept;
        inline const shared_ptr<Instance>& getInstance() const noexcept;
        inline const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities() const noexcept;
        inline const VkSurfaceFormatKHR& getSurfaceFormat() const noexcept;

        inline void setFormat(const VkSurfaceFormatKHR& format) noexcept;
        
        inline static shared_ptr<Surface> makeSurface(const Window& window,
                                                      const shared_ptr<Instance>& ptr_instance,
                                                      const PhysicalDevice& physical_device);
        
        inline static vector<VkSurfaceFormatKHR> getAllSurfaceFormats(const Surface& surface, const PhysicalDevice& physical_device);

    private:
        shared_ptr<Instance> _ptr_instance;
        VkSurfaceKHR _surface_handle;
        VkSurfaceCapabilitiesKHR _surface_capabilities;
        VkSurfaceFormatKHR _surface_format;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Surface::Surface(const Window& window, const shared_ptr<Instance>& ptr_instance, const PhysicalDevice& physical_device) :
        _ptr_instance(ptr_instance),
        _surface_handle(VK_NULL_HANDLE)
    {
        assert(SDL_Vulkan_CreateSurface(window._ptr_window, _ptr_instance->getHandle(), &_surface_handle) == SDL_TRUE);
        assert(_surface_handle);
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device.physical_device_handle, _surface_handle, &_surface_capabilities);
    }

    inline Surface::~Surface()
    {
        vkDestroySurfaceKHR(_ptr_instance->getHandle(), _surface_handle, nullptr);
    }

    inline shared_ptr<Surface> Surface::makeSurface(const Window& window, const shared_ptr<Instance>& ptr_instance, const PhysicalDevice& physical_device)
    {
        return make_shared<Surface>(window, ptr_instance, physical_device);
    }

    inline VkSurfaceKHR Surface::getSurfaceHandle() const noexcept
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

    inline vector<VkSurfaceFormatKHR> Surface::getAllSurfaceFormats(const Surface& surface, const PhysicalDevice& physical_device)
    {
        uint32_t num_formats = 0;

        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device.physical_device_handle, surface._surface_handle, &num_formats, nullptr);

        vector<VkSurfaceFormatKHR> formats (num_formats);

        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device.physical_device_handle, surface._surface_handle, &num_formats, formats.data());

        return formats;
    }
}

#endif /* Surface_hpp */
