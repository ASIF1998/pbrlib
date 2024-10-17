//
//  Window.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <SDL3/SDL.h>

#include <string>
#include <string_view>

#include <tuple>

#include <vector>
#include <memory>

namespace pbrlib
{
    class   Swapchain;
    class   Instance;
    struct  PhysicalDevice;
    class 	Device;
    class 	Window;

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

        /**
         * @enum Resizable.
         * @brief Перечисление указывающее возможность изменения размера окна.
        */
        enum class Resizable
        {
            DYNAMIC = SDL_WINDOW_RESIZABLE,
            STATIC  = 0
        };

        class Builder
        {
        public:
            Builder();

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)      = delete;
            Builder& operator = (const Builder&) = delete;

            Builder& setTitle(const std::string_view title);

            Builder& setWidth(int width)                noexcept;
            Builder& setHeight(int height)              noexcept;
            Builder& setExtend(int width, int height)   noexcept;

            Builder& setPositionX(int pos_x)            noexcept;
            Builder& setPositionY(int pos_y)            noexcept;
            Builder& setPosition(int pos_x, int pos_y)  noexcept;

            /**
             * @brief Метод устанавливающий параметр, указывающий возможность изменения размера окна.
             * 
             * @param resizable параметр, указывающий возможность изменения размера окна.
            */
            Builder& setResizableWindow(Resizable resizable) noexcept;

            Window                  build()     const;
            std::unique_ptr<Window> buildPtr()  const;

        private:
            std::string _title;
            int         _width;
            int         _height;
            int         _pos_x;
            int         _pos_y;
            Resizable   _resizable;
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
            const std::string_view  title, 
            int                     width, 
            int                     height, 
            int                     pos_x, 
            int                     pos_y, 
            Resizable               resizable = Resizable::STATIC
        );

        Window(Window&& window);
        Window(const Window&) = delete;

        ~Window();

        Window& operator = (Window&&);
        Window& operator = (const Window&) = delete;

        std::tuple<int, int> getExtent()         const;
        std::tuple<int, int> getDrawableExtent() const;

        std::string&        getTitle();
        const std::string&  getTitle() const;

        std::shared_ptr<Swapchain>          getSwapchain() noexcept;
        std::shared_ptr<const Swapchain>    getSwapchain() const noexcept;

        void setTitle(const std::string_view title);

        static bool showCursor(bool e)      noexcept;    
        static void captureMouse(bool e)    noexcept;

        static void getVulkanInstanceExtensions(const Window& window, std::vector<const char*>& out_extensions);

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
        static std::unique_ptr<Window> make(
            const std::string_view  title, 
            int                     width, 
            int                     height, 
            int                     pos_x, 
            int                     pos_y, 
            Resizable               resizable = Resizable::STATIC
        );

    public:
        static constexpr int WINDOW_POSITION_CENTERED = SDL_WINDOWPOS_CENTERED;

    private:
        void _initVulkanResources(
            const Instance*         ptr_instance, 
            const PhysicalDevice*   ptr_physical_device,
            const Device*           ptr_device,
            uint32_t                queue_family_index
        );
        
        bool _hasVulkanResources() const noexcept;

        std::shared_ptr<const Surface>  _getVulkanSurface()     noexcept;
        std::shared_ptr<Swapchain>      _getVulkanSwapchain()   noexcept;

    private:
        SDL_Window*                 _ptr_window;
        std::shared_ptr<Swapchain>  _ptr_swapchain;
        std::string                 _title;
    };
}

#endif /* Window_hpp */
