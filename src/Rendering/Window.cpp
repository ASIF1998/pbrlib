//
//  Window.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <algorithm>

#include <stdexcept>

#include <SDL2/SDL_vulkan.h>

#include "Window.hpp"

#include "VulkanWrapper/Instance.hpp"
#include "VulkanWrapper/PhysicalDevice.hpp"

#include "VulkanWrapper/Swapchain.hpp"

#include "VulkanWrapper/DeviceQueue.hpp"

#include "../Util/enumCast.hpp"

namespace pbrlib
{
    bool        Window::_is_init_SDL    = false;
    uint32_t    Window::_num_window     = 0;

    Window::Window(
        const string_view   title, 
        int                 width, 
        int                 height, 
        int                 pos_x, 
        int                 pos_y, 
        Window::Resizable   resizable
    ) :
        _ptr_window     (nullptr),
        _ptr_swapchain  (nullptr),
        _title          (title)
    {
        if (!_is_init_SDL) {
            if (SDL_Init(SDL_INIT_EVERYTHING)) {
                throw runtime_error(SDL_GetError());
            }
            
            _is_init_SDL = true;
        }

        _num_window++;

        _ptr_window = SDL_CreateWindow(
            title.data(), 
            pos_x, 
            pos_y, 
            width, 
            height, 
            SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | util::enumCast(resizable)
        );

        assert(_ptr_window);
    }

    Window::Window(Window&& window) 
    {
        swap(_ptr_window, window._ptr_window);
    }

    Window::~Window()
    {
        _num_window--;
        SDL_DestroyWindow(_ptr_window);

        if (!_num_window && _is_init_SDL) {
            SDL_Quit();
            _is_init_SDL = false;
        }
    }

    tuple<int, int> Window::getExtent() const
    {
        int width   = 0;
        int height  = 0;

        SDL_GetWindowSize(_ptr_window, &width, &height);

        return make_tuple(width, height);
    }

    tuple<int, int> Window::getDrawableExtent() const
    {
        int width   = 0;
        int height  = 0;

        SDL_Vulkan_GetDrawableSize(_ptr_window, &width, &height);

        return make_tuple(width, height);
    }

    string& Window::getTitle()
    {
        return _title;
    }

    const string& Window::getTitle() const
    {
        return _title;
    }

    PtrSwapchain& Window::getSwapchain() noexcept
    {
        return _ptr_swapchain;
    }

    const PtrSwapchain& Window::getSwapchain() const noexcept
    {
        return _ptr_swapchain;
    }

    void Window::setTitle(const string_view title) 
    {
        _title = title;
        SDL_SetWindowTitle(_ptr_window, _title.data());
    }

    void Window::_initVulkanResources(
        const PtrInstance&          ptr_instance, 
        const PtrPhysicalDevice&    ptr_physical_device,
        const PtrDevice&            ptr_device,
        uint32_t                    queue_family_index
    )
    {
        if (!_ptr_swapchain) {
            PtrSurface                  ptr_surface                 = Surface::make(*this, ptr_instance, ptr_physical_device);
            vector<VkSurfaceFormatKHR>  surface_supported_formats   = Surface::getAllSurfaceFormats(*ptr_surface, ptr_physical_device);
            
            VkSurfaceFormatKHR current_surface_format;

            for (size_t i{0}, num_surface_formats{surface_supported_formats.size()}; i < num_surface_formats; i++) {
                if (surface_supported_formats[i].colorSpace ==  VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && surface_supported_formats[i].format == VK_FORMAT_B8G8R8A8_UNORM) {
                    current_surface_format = surface_supported_formats[i];
                } else if (i == num_surface_formats) {
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

    PtrSurface& Window::_getVulkanSurface() noexcept
    {
        return _ptr_swapchain->getSurface();
    }

    PtrSwapchain& Window::_getVulkanSwapchain() noexcept
    {
        return _ptr_swapchain;
    }

    bool Window::showCursor(bool e) noexcept
    {
        return SDL_ShowCursor(e);
    }

    void Window::captureMouse(bool e) noexcept
    {
        auto es = (e ? SDL_TRUE : SDL_FALSE);
        SDL_CaptureMouse(es);
    }

    void Window::getVulkanInstanceExtensions(const Window& window, vector<const char*>& out_extensions)
    {
        uint32_t num_extensions_names = 0;

        assert(SDL_Vulkan_GetInstanceExtensions(window._ptr_window, &num_extensions_names, nullptr) == SDL_TRUE);
        uint32_t offset = static_cast<uint32_t>(out_extensions.size());
        out_extensions.resize(offset + num_extensions_names);
        assert(SDL_Vulkan_GetInstanceExtensions(window._ptr_window, &num_extensions_names, out_extensions.data() + offset) == SDL_TRUE);
    }

    PtrWindow Window::make(
        const string_view   title, 
        int                 width, 
        int                 height, 
        int                 pos_x, 
        int                 pos_y, 
        Window::Resizable   resizable
    )
    {
        return make_shared<Window>(title, width, height, pos_x, pos_y, resizable);
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

    void Window::Builder::setTitle(const string_view title)
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

    PtrWindow Window::Builder::buildPtr() const
    {
        return Window::make(_title, _width, _height, _pos_x, _pos_y, _resizable);
    }
}
