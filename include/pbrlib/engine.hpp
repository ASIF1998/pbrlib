#pragma once

#include <pbrlib/Rendering/Window.hpp>

#include <string_view>
#include <functional>
#include <optional>

namespace pbrlib
{
    struct  Config;
    class   Engine;
}

namespace pbrlib
{
    using SetupCallback = std::function<void(Engine* ptr_engine)>;
}

namespace pbrlib
{
    class Engine
    {
    public:
        explicit Engine(const Config& config);

        void setTitle(std::string_view title);
        void resize(uint32_t width, uint32_t height);

        void setupCallback(SetupCallback setup_callback);

#if 0
        void preRenderCallback(PreRenderCallback callback);
        void postRenderCallback(PostRenderCallback callback);
#endif

    private:
        std::optional<Window> _window;
    };
}