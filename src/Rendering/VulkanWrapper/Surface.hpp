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
    class Surface;

    using PtrSurface = shared_ptr<Surface>;

    class Surface
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param window            окно.
         * @param ptr_instance      указатель на экземпляр Vulkan'а.
         * @param physical_device   физическое устройтсво.
        */
        inline Surface(
            const Window&               window, 
            const PtrInstance&          ptr_instance, 
            const PhysicalDevice&       physical_device
        );

        inline Surface(Surface&& surface);
        Surface(const Surface&) = delete;
        
        inline ~Surface();

        Surface& operator = (Surface&&)         = delete;
        Surface& operator = (const Surface&)    = delete;

        inline const VkSurfaceKHR&              getSurfaceHandle()          const noexcept;
        inline PtrInstance&                     getInstance()               noexcept;
        inline const PtrInstance&               getInstance()               const noexcept;
        inline const VkSurfaceCapabilitiesKHR&  getSurfaceCapabilities()    const noexcept;
        inline const VkSurfaceFormatKHR&        getSurfaceFormat()          const noexcept;

        inline void setFormat(const VkSurfaceFormatKHR& format) noexcept;
        
        inline static PtrSurface make(
            const Window&               window,
            const PtrInstance&          ptr_instance,
            const PhysicalDevice&       physical_device
        );
        
        inline static vector<VkSurfaceFormatKHR> getAllSurfaceFormats(
            const Surface&          surface, 
            const PhysicalDevice&   physical_device
        );

    private:
        PtrInstance                 _ptr_instance;
        VkSurfaceKHR                _surface_handle;
        VkSurfaceCapabilitiesKHR    _surface_capabilities;
        VkSurfaceFormatKHR          _surface_format;
    };
}

#include "Surface.inl"

#endif /* Surface_hpp */
