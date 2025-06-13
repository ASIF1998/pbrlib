#include <pbrlib/rendering/window.hpp>

#include <pbrlib/exceptions.hpp>

#include <backend/profiling.hpp>

#include <SDL3/SDL.h>

#include <stdexcept>
#include <format>

namespace utils
{
    SDL_Window* cast(void* ptr_window)
    {
        return static_cast<SDL_Window*>(ptr_window);
    }
}

namespace pbrlib
{
    Window::Window(void* ptr_window) :
        _ptr_window(ptr_window)
    { }

    Window::Window(Window&& window)
    {
        std::swap(_ptr_window, window._ptr_window);
    }

    Window::~Window()
    {
        if (_ptr_window)
        {
            SDL_DestroyWindow(utils::cast(_ptr_window));
            SDL_Quit();
        }
    }

    Window& Window::operator = (Window&& window)
    {
        std::swap(_ptr_window, window._ptr_window);
        return *this;
    }

    std::string Window::title() const
    {
        return SDL_GetWindowTitle(utils::cast(_ptr_window));
    }

    void Window::title(std::string_view title)
    {
        SDL_SetWindowTitle(utils::cast(_ptr_window), title.data());
    }

    std::pair<int32_t, int32_t> Window::size() const
    {
        int width   = 0;
        int height  = 0;

        SDL_GetWindowSize(utils::cast(_ptr_window), &width, &height);

        return std::make_pair(width, height);
    }

    void Window::messageBox(std::string_view title, std::string_view msg) const
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title.data(), msg.data(), utils::cast(_ptr_window));
    }
}

namespace pbrlib
{
    Window::Builder& Window::Builder::title(std::string_view title)
    {
        _title = title;
        return *this;
    }

    Window::Builder& Window::Builder::size(uint32_t width, uint32_t height)
    {
        _width  = width;
        _height = height;

        return *this;
    }

    Window::Builder& Window::Builder::resizable(bool is_resizable)
    {
        _is_resizable = is_resizable;
        return *this;
    }

    void Window::Builder::validate()
    {
        if (_title.empty())
            throw exception::InvalidState("[window::builder] title is empty");

        if (_width == 0 || _height == 0)
            throw exception::InvalidState("[window::builder] size is 0"); 
    }

    Window Window::Builder::build()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        validate();

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            std::string error_msg = SDL_GetError();
            SDL_ClearError();

            throw exception::InitializeError(std::format("[window] SDL: {}", error_msg));
        }

        auto flags = SDL_WINDOW_VULKAN;
        if (_is_resizable) 
            flags |= SDL_WINDOW_RESIZABLE;

        auto ptr_sdl_window = SDL_CreateWindow(
            _title.data(), 
            static_cast<int>(_width), static_cast<int>(_height), 
            flags
        );

        if (!ptr_sdl_window) 
            throw exception::InitializeError("[window::builder] failed create window");

        return Window(ptr_sdl_window);
    }
}