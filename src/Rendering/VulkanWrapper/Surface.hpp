//
//  Surface.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include <vulkan/vulkan.h>

#include "../Window.hpp"

#include <memory>

#include <SDL_vulkan.h>

using namespace std;

namespace pbrlib
{
    class   Surface;
    struct  PhysicalDevice;
    class   Instance;

    using PtrSurface        = shared_ptr<Surface>;
    using PtrPhysicalDevice = shared_ptr<PhysicalDevice>;
    using PtrInstance       = shared_ptr<Instance>;

    class Surface
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param window                окно.
         * @param ptr_instance          указатель на экземпляр Vulkan'а.
         * @param ptr_physical_device   физическое устройство.
        */
        Surface(
            const Window&               window, 
            const PtrInstance&          ptr_instance, 
            const PtrPhysicalDevice&    ptr_physical_device
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
            const PtrPhysicalDevice&    ptr_physical_device
        );
        
        static vector<VkSurfaceFormatKHR> getAllSurfaceFormats(
            const Surface&              surface, 
            const PtrPhysicalDevice&    ptr_physical_device
        );

    private:
        PtrInstance                 _ptr_instance;
        VkSurfaceKHR                _surface_handle;
        VkSurfaceCapabilitiesKHR    _surface_capabilities;
        VkSurfaceFormatKHR          _surface_format;
    };
}

#endif /* Surface_hpp */
