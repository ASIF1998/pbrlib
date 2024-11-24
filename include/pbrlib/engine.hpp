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
    class   Scene;
    class   FrameGraph;
}

namespace pbrlib
{
    using SetupCallback = std::function<void(Engine* ptr_engine, Scene* ptr_scene)>;
}

namespace pbrlib
{
    class Engine final
    {
        void init(const Config& config);

    public:
        explicit Engine(const Config& config);

        Engine(Engine&& engine);
        Engine(const Engine& engine) = delete;

        ~Engine();

        Engine& operator = (Engine&& engine);
        Engine& operator = (const Engine&& engine) = delete;

        void resize(uint32_t width, uint32_t height);

        [[nodiscard]] Camera& camera() noexcept;
        [[nodiscard]] Window& window() noexcept;

        void setupCallback(SetupCallback setup_callback);

#if 0
        void preRenderCallback(PreRenderCallback callback);
        void postRenderCallback(PostRenderCallback callback);
#endif

    void run();

    private:
        std::optional<Window> _window;

        SetupCallback _setup_callback;

        Camera _camera;

        std::unique_ptr<FrameGraph> _ptr_frame_graph;
    };
}