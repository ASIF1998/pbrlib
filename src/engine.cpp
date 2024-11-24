#include <pbrlib/config.hpp>
#include <pbrlib/engine.hpp>

#include <pbrlib/scene/scene.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/frame_graph.hpp>

#include <SDL3/SDL.h>

#include <stdexcept>

namespace pbrlib
{
    Engine::Engine(const Config& config)
    {
        init(config);

        if (config.drawInWindow) {
            _window = Window::Builder()
                .title(config.title)
                .size(config.width, config.height)
                .resizable(false)
                .build();
            
            _ptr_frame_graph = std::make_unique<FrameGraph>(&_window.value());
        }
        else
            _ptr_frame_graph = std::make_unique<FrameGraph>(config.width, config.height);
    }

    Engine::Engine(Engine&& engine) :
        _setup_callback (engine._setup_callback),
        _camera         (engine._camera)
    {
        std::swap(_window, engine._window);
    }

    Engine::~Engine()
    {
        SDL_Quit();
    }

    Engine& Engine::operator = (Engine&& engine)
    {
        std::swap(_window, engine._window);

        _setup_callback = engine._setup_callback;
        _camera         = engine._camera;

        return *this;
    }

    void Engine::init(const Config& config)
    {
        static bool is_init = false;

        if (is_init)
            return ;

        if (config.drawInWindow && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
        {
            std::string error_msg = SDL_GetError();
            SDL_ClearError();

            throw std::runtime_error(std::format("Failed initialize SDL: {}", error_msg));
        }

        pbrlib::log::priv::EngineLogger::init();
        pbrlib::log::priv::AppLogger::init();

        is_init = true;
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

    Window& Engine::window() noexcept
    {
        return *_window;
    }
}