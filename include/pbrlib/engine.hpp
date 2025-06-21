#pragma once

#include <pbrlib/rendering/window.hpp>
#include <pbrlib/rendering/camera.hpp>

#include <string_view>
#include <functional>
#include <optional>
#include <memory>

namespace pbrlib
{
    struct  Config;
    class   Engine;
    struct  InputStay;
    class   Scene;
}

namespace pbrlib::backend
{
    class FrameGraph;
    class Canvas;
    class MaterialManager;
    class MeshManager;
}

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::testing
{
    class FrameGraphResourcesGetter;
}

namespace pbrlib
{
    using SetupCallback     = std::function<void(Engine& engine, Scene& scene)>;
    using UpdateCallback    = std::function<void (const InputStay& input_stay, float delta_time)>;
}

namespace pbrlib
{
    class Engine final
    {
        friend class Scene;
        friend class testing::FrameGraphResourcesGetter;

        static void updateInputState(InputStay& input_stay);

        void draw();
        void updateTime();

    public:
        explicit Engine(const Config& config);

        Engine(Engine&& engine)         = default;
        Engine(const Engine& engine)    = delete;

        ~Engine();

        Engine& operator = (Engine&& engine)        = default;
        Engine& operator = (const Engine&& engine)  = delete;

        void resize(uint32_t width, uint32_t height);

        [[nodiscard]] Camera& camera() noexcept;
        [[nodiscard]] Window& window();

        void setup(SetupCallback setup_callback);
        void update(UpdateCallback update_callback);

        void run();

    private:
        std::optional<Window> _window;

        SetupCallback   _setup_callback;
        UpdateCallback  _update_callback;

        Camera _camera;

        std::unique_ptr<backend::vk::Device> _ptr_device;
        std::unique_ptr<backend::FrameGraph> _ptr_frame_graph;

        std::unique_ptr<backend::MaterialManager>   _ptr_material_manager;
        std::unique_ptr<backend::MeshManager>       _ptr_mesh_manager;

        std::unique_ptr<Scene> _ptr_scene;

        std::unique_ptr<backend::Canvas> _ptr_canvas;

        float _delta_time = 0.0f;
    };
}