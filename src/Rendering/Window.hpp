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
    class Surface;
    class Swapchain;
    class Instance;
    class PhysicalDevice;
    class Device;
    class Window;

    using PtrSurface        = shared_ptr<Surface>;
    using PtrSwapchain      = shared_ptr<Swapchain>;
    using PtrInstance       = shared_ptr<Instance>;
    using PtrPhysicalDevice = shared_ptr<PhysicalDevice>;
    using PtrDevice         = shared_ptr<Device>;
    using PtrWindow         = shared_ptr<Window>;

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
        friend class SceneView;

    public:
        /**
         * @brief Используется с методом Window::getExtent() и Window::getDrawableExtent().
        */
        enum
        {
            WINDOW_WIDTH    = 0,
            WINDOW_HEIGHT   = 1
        };

        class Builder
        {
        public:
            Builder();

            void setTitle(const string_view title);

            void setWidth(int width)                noexcept;
            void setHeight(int height)              noexcept;
            void setExtend(int width, int height)   noexcept;

            void setPositionX(int pos_x)            noexcept;
            void setPositionY(int pos_y)            noexcept;
            void setPosition(int pos_x, int pos_y)  noexcept;

            /**
             * @brief Метод устанавливающий параметр, указывающий возможность изменения размера окна.
             * 
             * @param resizable параметр, указывающий возможность изменения размера окна.
            */
            void setResizableWindow(ResizableWindow resizable) noexcept;

            Window      build()     const;
            PtrWindow   buildPtr()  const;

        private:
            string          _title;
            int             _width;
            int             _height;
            int             _pos_x;
            int             _pos_y;
            ResizableWindow _resizable;
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
         * @param resizable параметр, указывающий возможность изменения размера окна.
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

        PtrSwapchain&       getSwapchain() noexcept;
        const PtrSwapchain& getSwapchain() const noexcept;

        void setTitle(const string_view title);

        static bool showCursor(bool e)      noexcept;    
        static void captureMouse(bool e)    noexcept;

        static void getVulkanInstanceExtensions(const Window& window, vector<const char*>& out_extensions);

        /**
         * @brief Статический метод, создающий окно.
         * 
         * @param title     заголовок окна.
         * @param width     ширина окна.
         * @param height    высота окна.
         * @param pos_x     позиция окна по координате x.
         * @param pos_y     позиция окна по координате y.
         * @param resizable параметр указывающий возможность изменения размера окна.
         * @return Указатель на окно.
        */
        static PtrWindow make(
            const string_view   title, 
            int                 width, 
            int                 height, 
            int                 pos_x, 
            int                 pos_y, 
            ResizableWindow     resizable = ResizableWindow::STATIC
        );

    public:
        static constexpr int WINDOW_POSITION_CENTERED = SDL_WINDOWPOS_CENTERED;

    private:
        void _initVulkanResources(
            const PtrInstance&          ptr_instance, 
            const PtrPhysicalDevice&    ptr_physical_device,
            const PtrDevice&            ptr_device,
            uint32_t                    queue_family_index
        );
        bool _hasVulkanResources() const noexcept;

        PtrSurface&     _getVulkanSurface()     noexcept;
        PtrSwapchain&   _getVulkanSwapchain()   noexcept;

    private:
        SDL_Window*     _ptr_window;
        PtrSwapchain    _ptr_swapchain;
        string          _title;

        static bool     _is_init_SDL;
        static uint32_t _num_window;
    };
}

#endif /* Window_hpp */
