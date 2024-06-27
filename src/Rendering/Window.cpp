//
//  Window.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <algorithm>

#include <stdexcept>
#include <cassert>

#include <SDL3/SDL_vulkan.h>

#include <pbrlib/Rendering/Window.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Instance.hpp>
#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Swapchain.hpp>

#include <pbrlib/Rendering/VulkanWrapper/DeviceQueue.hpp>

#include <pbrlib/Util/enumCast.hpp>

namespace pbrlib
{
    Window::Window(
        const std::string_view  title, 
        int                     width, 
        int                     height, 
        int                     pos_x, 
        int                     pos_y, 
        Window::Resizable       resizable
    ) :
        _ptr_window     (nullptr),
        _ptr_swapchain  (nullptr),
        _title          (title)
    {
        _ptr_window = SDL_CreateWindow(title.data(), width, height, SDL_WINDOW_VULKAN | utils::enumCast(resizable));

        assert(_ptr_window);
    }

    Window::Window(Window&& window) 
    {
        std::swap(_ptr_window, window._ptr_window);
    }

    Window::~Window()
    {
        SDL_DestroyWindow(_ptr_window);
    }

    std::tuple<int, int> Window::getExtent() const
    {
        int width   = 0;
        int height  = 0;

        SDL_GetWindowSize(_ptr_window, &width, &height);

        return std::make_tuple(width, height);
    }

    std::tuple<int, int> Window::getDrawableExtent() const
    {
        int width   = 0;
        int height  = 0;

        SDL_GetWindowSizeInPixels(_ptr_window, &width, &height);        
        

        return std::make_tuple(width, height);
    }

    std::string& Window::getTitle()
    {
        return _title;
    }

    const std::string& Window::getTitle() const
    {
        return _title;
    }

    std::shared_ptr<Swapchain> Window::getSwapchain() noexcept
    {
        return _ptr_swapchain;
    }

    std::shared_ptr<const Swapchain> Window::getSwapchain() const noexcept
    {
        return _ptr_swapchain;
    }

    void Window::setTitle(const std::string_view title) 
    {
        _title = title;
        SDL_SetWindowTitle(_ptr_window, _title.data());
    }

    void Window::_initVulkanResources(
        const Instance*         ptr_instance, 
        const PhysicalDevice*   ptr_physical_device,
        const Device*           ptr_device,
        uint32_t                queue_family_index
    )
    {
        if (!_ptr_swapchain) {
            std::shared_ptr                 ptr_surface                 = Surface::make(*this, ptr_instance, ptr_physical_device);
            std::vector<VkSurfaceFormatKHR> surface_supported_formats   = Surface::getAllSurfaceFormats(*ptr_surface, ptr_physical_device);
            
            VkSurfaceFormatKHR current_surface_format;

            for (size_t i{0}, num_surface_formats{surface_supported_formats.size()}; i < num_surface_formats; i++) {
                if (surface_supported_formats[i].colorSpace ==  VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && surface_supported_formats[i].format == VK_FORMAT_B8G8R8A8_UNORM) {
                    current_surface_format = surface_supported_formats[i];
                    i = num_surface_formats;
                } else if (i == num_surface_formats - 1) {
                    current_surface_format = surface_supported_formats[0];
                }
            }

            ptr_surface->setFormat(current_surface_format);

            if (DeviceQueue::isPresentSuppoerted(queue_family_index, *ptr_physical_device, *ptr_surface)) {
                _ptr_swapchain = Swapchain::make(
                    ptr_device, 
                    queue_family_index, 
                    ptr_surface
                );
            }
        }
    }

    bool Window::_hasVulkanResources() const noexcept
    {
        return _ptr_swapchain != nullptr;
    }

    std::shared_ptr<const Surface> Window::_getVulkanSurface() noexcept
    {
        return _ptr_swapchain->getSurface();
    }

    std::shared_ptr<Swapchain> Window::_getVulkanSwapchain() noexcept
    {
        return _ptr_swapchain;
    }

    bool Window::showCursor(bool e) noexcept
    {
        // return SDL_ShowCursor(e);
        return true;
    }

    void Window::captureMouse(bool e) noexcept
    {
        auto es = (e ? SDL_TRUE : SDL_FALSE);
        SDL_CaptureMouse(es);
    }

    void Window::getVulkanInstanceExtensions(const Window& window, std::vector<const char*>& out_extensions)
    {
        uint32_t    num_extensions_names    = 0;
        auto        ptr_extensions_names    = SDL_Vulkan_GetInstanceExtensions(&num_extensions_names);

        auto offset = out_extensions.size();
        out_extensions.resize(offset + num_extensions_names);
        
        for (size_t i = 0; i < num_extensions_names; ++i)
            out_extensions[offset + i] = ptr_extensions_names[i];
    }

    std::unique_ptr<Window> Window::make(
        const std::string_view  title, 
        int                     width, 
        int                     height, 
        int                     pos_x, 
        int                     pos_y, 
        Window::Resizable       resizable
    )
    {
        return make_unique<Window>(title, width, height, pos_x, pos_y, resizable);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Window::Builder::Builder() :
        _title      ("Window"),
        _width      (0),
        _height     (0),
        _pos_x      (0),
        _pos_y      (0),
        _resizable  (Resizable::STATIC)
    {}

    void Window::Builder::setTitle(const std::string_view title)
    {
        _title = title;
    }

    void Window::Builder::setWidth(int width) noexcept
    {
        _width = width;
    }

    void Window::Builder::setHeight(int height) noexcept
    {
        _height = height;
    }

    void Window::Builder::setExtend(int width, int height) noexcept
    {
        _width  = width; 
        _height = height;
    }

    void Window::Builder::setPositionX(int pos_x) noexcept
    {
        _pos_x = pos_x;
    }

    void Window::Builder::setPositionY(int pos_y) noexcept
    {
        _pos_y = pos_y;
    }

    void Window::Builder::setPosition(int pos_x, int pos_y) noexcept
    {
        _pos_x = pos_x;
        _pos_y = pos_y;
    }

    void Window::Builder::setResizableWindow(Window::Resizable resizable) noexcept
    {
        _resizable = resizable;
    }

    Window Window::Builder::build() const
    {
        return Window(_title, _width, _height, _pos_x, _pos_y, _resizable);
    }

    std::unique_ptr<Window> Window::Builder::buildPtr() const
    {
        return Window::make(_title, _width, _height, _pos_x, _pos_y, _resizable);
    }
}
