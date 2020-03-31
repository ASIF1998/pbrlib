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
        Surface(
            const Window&               window, 
            const PtrInstance&          ptr_instance, 
            const PhysicalDevice&       physical_device
        );

        Surface(Surface&& surface);
        Surface(const Surface&) = delete;
        
        ~Surface();

        Surface& operator = (Surface&&)         = delete;
        Surface& operator = (const Surface&)    = delete;

        const VkSurfaceKHR&              getSurfaceHandle()          const noexcept;
        PtrInstance&                     getInstance()               noexcept;
        const PtrInstance&               getInstance()               const noexcept;
        const VkSurfaceCapabilitiesKHR&  getSurfaceCapabilities()    const noexcept;
        const VkSurfaceFormatKHR&        getSurfaceFormat()          const noexcept;

        void setFormat(const VkSurfaceFormatKHR& format) noexcept;
        
        static PtrSurface make(
            const Window&               window,
            const PtrInstance&          ptr_instance,
            const PhysicalDevice&       physical_device
        );
        
        static vector<VkSurfaceFormatKHR> getAllSurfaceFormats(
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

#endif /* Surface_hpp */
