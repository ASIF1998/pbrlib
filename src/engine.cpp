#include <pbrlib/config.hpp>
#include <pbrlib/engine.hpp>

#include <pbrlib/scene/scene.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/frame_graph.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <SDL3/SDL.h>

#include <stdexcept>

namespace pbrlib
{
    uint8_t Engine::num_engine_instances = 0;
}

namespace pbrlib
{
    Engine::Engine(const Config& config) :
        _scene (config.title)
    {
        if (num_engine_instances > 0)
            throw std::runtime_error("The Engine class must exist in a single instance");

        ++num_engine_instances;

        pbrlib::log::priv::EngineLogger::init();
        pbrlib::log::priv::AppLogger::init();

        if (config.drawInWindow && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
        {
            std::string error_msg = SDL_GetError();
            SDL_ClearError();

            throw std::runtime_error(std::format("Failed initialize SDL: {}", error_msg));
        }

        _ptr_device = std::make_unique<vk::Device>();

        if (config.drawInWindow) {
            _window = Window::Builder()
                .title(config.title)
                .size(config.width, config.height)
                .resizable(false)
                .build();
            
            _ptr_device->init(&_window.value());
            _ptr_frame_graph = std::make_unique<FrameGraph>(_ptr_device.get(), &_window.value());
        }
        else
        {
            _ptr_device->init(nullptr);
            _ptr_frame_graph = std::make_unique<FrameGraph>(_ptr_device.get(), config.width, config.height);
        }
    }

    Engine::~Engine()
    {
        log::engine::info("Destroy engine");

        SDL_Quit();

        --num_engine_instances;
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
        if (_setup_callback)
            _setup_callback(this, &_scene);        
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