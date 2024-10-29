#pragma once

#include <pbrlib/new-renderer/window.hpp>

#include <pbrlib/new-renderer/camera.hpp>

#include <string_view>
#include <functional>
#include <optional>

/// @todo remove
#include <pbrlib/core.hpp>

namespace pbrlib
{
    struct  Config;
    class   Engine;
    class   Scene;
}

namespace pbrlib
{
    using SetupCallback = std::function<void(Engine* ptr_engine, Scene* ptr_scene)>;
}

namespace pbrlib
{
    class Engine final
    {
        void init();

    public:
        explicit Engine(const Config& config);

        ~Engine();

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
    };
}