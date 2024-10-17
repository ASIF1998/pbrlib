#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

namespace pbrlib
{
    Engine::Engine(const Config& config)
    {
        _window = Window::Builder()
            .setExtend(config.width, config.height)
            .setPosition(Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED)
            .setResizableWindow(Window::Resizable::STATIC)
            .setTitle(config.title)
            .build();
    }

    void Engine::setTitle(std::string_view title)
    {
        _window->setTitle(title);
    }

    void Engine::resize(uint32_t width, uint32_t height)
    {
        /// @todo impl
    }

    void Engine::setupCallback(SetupCallback callback)
    {
        callback(this);
    }
}