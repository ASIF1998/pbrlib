#include <pbrlib/config.hpp>
#include <pbrlib/engine.hpp>

#include <pbrlib/scene/scene.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/frame_graph/frame_graph.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/scene/material_manager.hpp>
#include <backend/scene/mesh_manager.hpp>

#include <backend/renderer/canvas.hpp>

#include <backend/components.hpp>

#include <backend/profiling.hpp>

#include <backend/utils/align_size.hpp>

#include <pbrlib/input/input_stay.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/event_system.hpp>
#include <backend/events.hpp>

#include <backend/initialize.hpp>

#include <SDL3/SDL.h>

#include <stdexcept>

#include <chrono>

namespace pbrlib
{
    constinit uint8_t g_num_engine_instances = 0;
}

namespace pbrlib
{
    Engine::Engine(const Config& config)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (g_num_engine_instances > 0) [[unlikely]]
            throw exception::InvalidState("[engine] the Engine class must exist in a single instance");

        ++g_num_engine_instances;

        backend::initialize();

        EventSystem::emmit(backend::events::Initialize());

        _ptr_device = std::make_unique<backend::vk::Device>();
        _ptr_device->init();

        const uint32_t groupSize    = _ptr_device->workGroupSize();
        const uint32_t width        = backend::utils::alignSize(config.width, groupSize);
        const uint32_t height       = backend::utils::alignSize(config.height, groupSize);

        if (config.draw_in_window) [[likely]]
        {
            _window = Window::Builder()
                .title(config.title)
                .size(width, height)
                .resizable(false)
                .build();

            _ptr_canvas = std::make_unique<backend::Canvas>(*_ptr_device, &_window.value());
        }
        else 
            _ptr_canvas = std::make_unique<backend::Canvas>(*_ptr_device, width, height);

        _ptr_material_manager   = std::make_unique<backend::MaterialManager>(*_ptr_device);
        _ptr_mesh_manager       = std::make_unique<backend::MeshManager>(*_ptr_device);
        _ptr_scene              = std::make_unique<Scene>(config.title);
        _ptr_frame_graph        = std::make_unique<backend::FrameGraph>(*_ptr_device, config, *_ptr_canvas, *_ptr_material_manager, *_ptr_mesh_manager);

        _ptr_scene->meshManager(_ptr_mesh_manager.get());
    }

    Engine::~Engine()
    {
        backend::log::info("[engine] finalize");
        EventSystem::emmit(backend::events::Finalize());

        --g_num_engine_instances;
    }

    void Engine::resize(uint32_t width, uint32_t height)
    {
        /// @todo impl
    }

    void Engine::setup(SetupCallback callback)
    {
        _setup_callback = callback;
    }

    void Engine::update(UpdateCallback update_callback)
    {
        _update_callback = update_callback;
    }

    void Engine::run()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (_setup_callback) [[likely]]
        {
            _setup_callback(*_ptr_scene);
            _setup_callback = nullptr;
        }

        InputStay input_stay;

        bool is_close = true;

        do
        {
            updateInputState(input_stay);

            if (_window) [[likely]]
                is_close = input_stay.window.isClose();

#ifdef PBRLIB_ENABLE_DEVELOPER_MODE
            if (input_stay.keyboard.isDown(pbrlib::Keycode::F5)) [[likely]]
                _ptr_frame_graph->rebuildPasses();
#endif

            updateTime();

            if (_update_callback) [[likely]]
                _update_callback(*this, input_stay, _delta_time);

            _ptr_scene->update(input_stay, _delta_time);

            _ptr_material_manager->update();
            _ptr_mesh_manager->update();

            draw();

        } while (!is_close);
    }

    Camera& Engine::camera() noexcept
    {
        return _camera;
    }

    Window& Engine::window()
    {
        if (!_window) [[unlikely]]
            throw exception::InvalidState("[engine] window wasn't created");

        return *_window;
    }

    void Engine::updateInputState(InputStay& input_stay)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        input_stay.reset();

        SDL_Event event;
        while (SDL_PollEvent(&event))
            input_stay.add(&event);
    }

    void Engine::draw()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        std::vector<const SceneItem*> items;

        auto view = _ptr_scene->_registry.view<backend::components::Renderable>();

        for (auto entity: view)
            items.push_back(view.get<backend::components::Renderable>(entity).ptr_item);

        if (_ptr_frame_graph) [[likely]]
            _ptr_frame_graph->draw(_camera, items);
    }

    void Engine::updateTime()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        using msFloat = std::chrono::duration<float, std::milli>;

        static constinit std::chrono::high_resolution_clock timer;

        constexpr auto seconds_per_milisceond = 0.001f;

        static auto begin   = timer.now();
        static auto end     = begin;

        end = timer.now();

        _delta_time = std::chrono::duration_cast<msFloat>(end - begin).count() * seconds_per_milisceond;

        begin = end;
    }

    void Engine::update(const Config& config)
    {
        if (_ptr_frame_graph) [[likely]]
            _ptr_frame_graph->update(config);
    }
}
