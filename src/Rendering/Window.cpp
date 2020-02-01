//
//  Window.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <algorithm>

#include <stdexcept>

#include "Window.hpp"

namespace pbrlib
{
    bool Window::_is_init_SDL = false;
    uint32_t Window::_num_window = 0;

    Window::Window(const string_view title, int width, int height, int pos_x, int pos_y, ResizableWindow resizable) :
        _ptr_window(nullptr),
        _title(title)
    {
        if (!_is_init_SDL) {
            if (SDL_Init(SDL_INIT_EVERYTHING)) {
                throw runtime_error(SDL_GetError());
            }
        }

        _num_window++;

        /**
         *  TODO: Vulkan.
        */
        _ptr_window = SDL_CreateWindow( title.data(), 
                                        pos_x, 
                                        pos_y, 
                                        width, 
                                        height, 
                                        SDL_WINDOW_SHOWN | 
                                        static_cast<decltype(SDL_WINDOW_SHOWN)>(resizable));

        assert(_ptr_window);
    }

    inline Window::Window(Window&& window) 
    {
        swap(_ptr_window, window._ptr_window);
    }

    Window::~Window()
    {
        _num_window--;
        SDL_DestroyWindow(_ptr_window);

        if (!_num_window && _is_init_SDL) {
            SDL_Quit(); 
        }
    }

    tuple<int, int> Window::getExtent() const
    {
        int width = 0;
        int height = 0;

        SDL_GetWindowSize(_ptr_window, &width, &height);

        return make_tuple(width, height);
    }

    tuple<int, int> Window::getDrawableExtent() const
    {
        int width = 0;
        int height = 0;

        SDL_GL_GetDrawableSize(_ptr_window, &width, &height);

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

    void Window::setTitle(const string_view title) 
    {
        _title = title;
        SDL_SetWindowTitle(_ptr_window, _title.data());
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
}
