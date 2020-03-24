//
//  Window.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <SDL2/SDL.h>

#include <string>
#include <string_view>

#include <tuple>

#include <vector>

using namespace std;

namespace pbrlib
{
    /**
     * @enum ResizableWindow.
     * @brief Перечисление указывающее возможность изменения размера окна.
    */
    enum class ResizableWindow
    {
        DYNAMIC = SDL_WINDOW_RESIZABLE,
        STATIC  = 0
    };

    /**
     * @class Window.
    */
    class Window
    {
        friend class Surface;

    public:
        /**
         * @brief Используется с методом Window::getExtent() и Window::getDrawableExtent().
        */
        enum
        {
            WINDOW_WIDTH    = 0,
            WINDOW_HEIGHT   = 1
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param title     заголовок окна.
         * @param width     ширина окна.
         * @param height    высота окна.
         * @param pos_x     позиция окна по координате x.
         * @param pos_y     позиция окна по координате y.
         * @param resizable параметр указывающий возможность изменения размера окна.
        */
        Window(
            const string_view   title, 
            int                 width, 
            int                 height, 
            int                 pos_x, 
            int                 pos_y, 
            ResizableWindow     resizable = ResizableWindow::STATIC
        );

        Window(Window&& window);
        Window(const Window&) = delete;

        ~Window();

        Window& operator = (const Window&)  = delete;
        Window& operator = (Window&&)       = delete;

        tuple<int, int> getExtent()         const;
        tuple<int, int> getDrawableExtent() const;

        string&         getTitle();
        const string&   getTitle() const;

        void setTitle(const string_view title);

        static bool showCursor(bool e)      noexcept;    
        static void captureMouse(bool e)    noexcept;

        static void getVulkanInstanceExtensions(const Window& window, vector<const char*>& out_extensions);

    public:
        static constexpr int WINDOW_POSITION_CENTERED = SDL_WINDOWPOS_CENTERED;

    private:
        SDL_Window* _ptr_window;
        string      _title;

        static bool     _is_init_SDL;
        static uint32_t _num_window;
    };
}

#endif /* Window_hpp */
