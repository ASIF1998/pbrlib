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

#include <pbrlib/input/input_stay.hpp>

#include <pbrlib/scene/scene.hpp>

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

        backend::log::priv::EngineLogger::init();
        backend::log::priv::AppLogger::init();

        if (config.draw_in_window) [[likely]]
        {
            _window = Window::Builder()
                .title(config.title)
                .size(config.width, config.height)
                .resizable(false)
                .build();
        }

        _ptr_device = std::make_unique<backend::vk::Device>();
        _ptr_device->init();

        const auto ptr_window = _window ? &_window.value() : nullptr;

        _ptr_canvas = std::make_unique<backend::Canvas>(*_ptr_device, ptr_window, config);

        _ptr_material_manager   = std::make_unique<backend::MaterialManager>(*_ptr_device);
        _ptr_mesh_manager       = std::make_unique<backend::MeshManager>(*_ptr_device);
        _ptr_scene              = std::make_unique<Scene>(config.title);
        _ptr_frame_graph        = std::make_unique<backend::FrameGraph>(*_ptr_device, *_ptr_canvas, *_ptr_material_manager, *_ptr_mesh_manager);

        _ptr_scene->meshManager(_ptr_mesh_manager.get());
    }

    Engine::~Engine()
    {
        backend::log::info("[engine] finalize");

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
            _setup_callback(*this, *_ptr_scene);
            _setup_callback = nullptr;
        }

        InputStay input_stay;

        bool is_close = true;

        backend::log::info("[engine] initialize is done");

        do
        {
            updateInputState(input_stay);

            if (_window) [[likely]]
                is_close = input_stay.window.isClsoe();

#ifdef PBRLIB_ENABLE_DEVELOPER_MODE
            if (input_stay.keyboard.isDown(pbrlib::Keycode::F5)) [[likely]]
                _ptr_frame_graph->rebuildPasses();
#endif

            updateTime();

            if (_update_callback) [[likely]]
                _update_callback(input_stay, _delta_time);

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

        auto view = _ptr_scene->_registry.view<backend::component::Renderable>();

        for (auto entity: view)
            items.push_back(view.get<backend::component::Renderable>(entity).ptr_item);

        std::sort(std::begin(items), std::end(items), [this] (const SceneItem* item_1, const SceneItem* item_2)
        {
            const auto& bbox_1 = item_1->getComponent<backend::component::Renderable>().bbox;
            const auto& bbox_2 = item_2->getComponent<backend::component::Renderable>().bbox;

            const auto transform_1 = item_1->getComponent<component::Transform>();
            const auto transform_2 = item_2->getComponent<component::Transform>();

            const auto p1 = _camera.view() * transform_1.transform * ((bbox_1.p_min + bbox_1.p_max) * 0.5);
            const auto p2 = _camera.view() * transform_2.transform * ((bbox_2.p_min + bbox_2.p_max) * 0.5);

            return p1.z < p1.z;
        });

        if (_ptr_frame_graph) [[likely]]
            _ptr_frame_graph->draw(_camera, items);
    }

    void Engine::updateTime()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        static constinit std::chrono::high_resolution_clock timer;

        constexpr auto seconds_per_milisceond = 0.001f;

        static auto begin   = timer.now();
        static auto end     = begin;

        end = timer.now();

        _delta_time = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - begin).count() * seconds_per_milisceond;

        begin = end;
    }
}
