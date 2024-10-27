#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/logger/engine/logger.hpp>

#include <stdexcept>

namespace pbrlib
{
    Engine::Engine(const Config& config)
    {
        init();

        _window = Window::Builder()
            .setExtend(config.width, config.height)
            .setPosition(Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED)
            .setResizableWindow(Window::Resizable::STATIC)
            .setTitle(config.title)
            .build();
    }

    Engine::~Engine()
    {
        SDL_Quit();
    }

    void Engine::init()
    {
        static bool is_init = false;

        if (is_init)
            return ;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
        {
            std::string error_msg = SDL_GetError();
            SDL_ClearError();
            pbrlib::log::engine::error("Failed initialize SDL3: {}", error_msg);

            std::runtime_error("Failed initialize");
        }

        is_init = true;
    }

    void Engine::title(std::string_view title)
    {
        _window->setTitle(title);
    }

    void Engine::resize(uint32_t width, uint32_t height)
    {
        /// @todo impl
    }

    void Engine::setupCallback(SetupCallback callback)
    {
        _setup_callback = callback;
    }

    void Engine::run()
    {
        /// @todo add ptr to scene
        if (_setup_callback)
            _setup_callback(this, nullptr);        
    }

    Camera& Engine::camera() noexcept
    {
        return _camera;
    }
}