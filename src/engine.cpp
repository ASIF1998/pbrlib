#include <pbrlib/config.hpp>
#include <pbrlib/engine.hpp>

#include <pbrlib/scene/scene.hpp>
#include <backend/scene/renderable_component.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/frame_graph/frame_graph.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <pbrlib/input/input_stay.hpp>

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

        if (config.drawInWindow) 
        {
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
            {
                std::string error_msg = SDL_GetError();
                SDL_ClearError();

                throw std::runtime_error(std::format("Failed initialize SDL: {}", error_msg));
            }

            _window = Window::Builder()
                .title(config.title)
                .size(config.width, config.height)
                .resizable(false)
                .build();
        }

        _ptr_device = std::make_unique<vk::Device>();
        _ptr_device->init();

        if (_window)
            _ptr_frame_graph = std::make_unique<FrameGraph>(_ptr_device.get(), &_window.value());
        else 
            _ptr_frame_graph = std::make_unique<FrameGraph>(_ptr_device.get(), config);
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

        InputStay input_stay;

        bool is_close = true;

        do 
        {
            updateInputState(input_stay);

            if (_window)
                is_close = input_stay.window.isClsoe();

            draw();

        } while (!is_close);
    }

    Camera& Engine::camera() noexcept
    {
        return _camera;
    }

    Window& Engine::window() noexcept
    {
        return *_window;
    }

    void Engine::updateInputState(InputStay& input_stay)
    {
        input_stay.reset();

        SDL_Event event;
        while (SDL_PollEvent(&event))
            input_stay.add(&event);
    }

    void Engine::draw()
    {
        /// @todo добавить сортировку по глублине в пространстве камеры
        std::vector<const SceneItem*> items;

        auto view = _scene._registry.view<RenderableComponent>();

        for (auto entity: view)
            items.push_back(view.get<RenderableComponent>(entity).ptr_item);

        if (_ptr_frame_graph)
            _ptr_frame_graph->draw(items);
    }
}