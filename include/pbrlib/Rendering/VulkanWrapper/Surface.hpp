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

#include <pbrlib/Rendering/Window.hpp>

#include <SDL3/SDL_vulkan.h>

#include <vector>
#include <memory>

namespace pbrlib
{
    struct  PhysicalDevice;
    class   Instance;
}

namespace pbrlib
{
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
            const Window&           window, 
            const Instance*         ptr_instance, 
            const PhysicalDevice*   ptr_physical_device
        );

        Surface(Surface&& surface);
        Surface(const Surface&) = delete;
        
        ~Surface();

        Surface& operator = (Surface&&)         = delete;
        Surface& operator = (const Surface&)    = delete;

        const VkSurfaceKHR&             getSurfaceHandle()          const noexcept;
        const Instance*                 getInstance()               const noexcept;
        const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities()    const noexcept;
        const VkSurfaceFormatKHR&       getSurfaceFormat()          const noexcept;

        void setFormat(const VkSurfaceFormatKHR& format) noexcept;
        
        static std::unique_ptr<Surface> make(
            const Window&           window,
            const Instance*         ptr_instance,
            const PhysicalDevice*   ptr_physical_device
        );
        
        static std::vector<VkSurfaceFormatKHR> getAllSurfaceFormats(const Surface& surface, const PhysicalDevice* ptr_physical_device);

    private:
        const Instance*                     _ptr_instance;
        VkSurfaceKHR                        _surface_handle;
        VkSurfaceCapabilitiesKHR            _surface_capabilities;
        VkSurfaceFormatKHR                  _surface_format;
    };
}

#endif /* Surface_hpp */
