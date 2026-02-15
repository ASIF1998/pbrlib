#include <pbrlib/input/input_stay.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>

#include <backend/input/sdl_keycode_map.hpp>

namespace pbrlib::utils
{
    const SDL_Event* cast(EventHandle event_handle)
    {
        return reinterpret_cast<const SDL_Event*>(event_handle);
    }
}

namespace pbrlib
{
    void KeyboardStay::reset()
    {
        _pressed_map.fill(KeyboardStay::KeyStay::eNone);
    }

    void KeyboardStay::add(EventHandle event_handle)
    {
        auto ptr_event  = utils::cast(event_handle);
        auto keycode    = backend::utils::sdl_to_pbrlib_keycode.find(ptr_event->key.key);

        if (keycode != std::end(backend::utils::sdl_to_pbrlib_keycode))
        {
            auto keycode_index = backend::utils::enumCast(keycode->second);

            if (ptr_event->type == SDL_EVENT_KEY_DOWN)
                _pressed_map[keycode_index] = KeyStay::eDown;
            else if (ptr_event->type == SDL_EVENT_KEY_UP)
                _pressed_map[keycode_index] = KeyStay::eUp;
        }
    }

    bool KeyboardStay::isDown(Keycode key_code) const noexcept
    {
        if (key_code == Keycode::eCount) [[unlikely]]
            return false;

        return _pressed_map[backend::utils::enumCast(key_code)] == KeyStay::eDown;
    }

    bool KeyboardStay::isUp(Keycode key_code) const noexcept
    {
        if (key_code == Keycode::eCount) [[unlikely]]
            return false;

        return _pressed_map[backend::utils::enumCast(key_code)] == KeyStay::eUp;
    }
}

namespace pbrlib
{
    void WindowStay::add(EventHandle event_handle)
    {
        auto ptr_event = utils::cast(event_handle);

        if (ptr_event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) [[unlikely]]
            _is_close = true;

        if (ptr_event->type == SDL_EVENT_WINDOW_RESIZED) [[unlikely]]
            _is_resized = true;
    }

    void WindowStay::reset() noexcept
    {
        _is_close   = false;
        _is_resized = false;
    }

    bool WindowStay::isClose() const noexcept
    {
        return _is_close;
    }

    bool WindowStay::isResized() const noexcept
    {
        return _is_resized;
    }
}

namespace pbrlib
{
    void MouseButtonsStay::add(EventHandle event_handle)
    {
        auto ptr_event = utils::cast(event_handle);

        size_t button_index = ptr_event->button.button;

        if (button_index < SDL_BUTTON_LEFT || button_index > SDL_BUTTON_RIGHT)
            return;

        --button_index;

        if (ptr_event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            _pressed_map[button_index] = ButtonStay::eDown;
        else if (ptr_event->type == SDL_EVENT_MOUSE_BUTTON_UP)
            _pressed_map[button_index] = ButtonStay::eUp;
    }

    bool MouseButtonsStay::isDown(MouseButton button) const noexcept
    {
        return _pressed_map[backend::utils::enumCast(button)] == ButtonStay::eDown;
    }

    bool MouseButtonsStay::isUp(MouseButton button) const noexcept
    {
        return _pressed_map[backend::utils::enumCast(button)] == ButtonStay::eUp;
    }
}

namespace pbrlib
{
    void MouseMotionStay::update(EventHandle event_handle)
    {
        auto ptr_event = utils::cast(event_handle);

        if (ptr_event->type == SDL_EVENT_MOUSE_MOTION)
        {
            _current_coord.x = ptr_event->motion.x;
            _current_coord.y = ptr_event->motion.y;

            _relative_motion.x = ptr_event->motion.xrel;
            _relative_motion.y = ptr_event->motion.yrel;

            _is_motion = true;
        }
    }

    void MouseMotionStay::reset() noexcept
    {
        _is_motion = false;
    }

    math::vec2 MouseMotionStay::currentCoord() const noexcept
    {
        return _current_coord;
    }

    math::vec2 MouseMotionStay::relativeMotion() const noexcept
    {
        return _relative_motion;
    }

    bool MouseMotionStay::isMotion() const noexcept
    {
        return _is_motion;
    }
}

namespace pbrlib
{
    void InputStay::add(EventHandle event_handle)
    {
        auto ptr_event = utils::cast(event_handle);

        switch (ptr_event->type)
        {
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                keyboard.add(ptr_event);
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            case SDL_EVENT_WINDOW_RESIZED:
                window.add(ptr_event);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouse_buttons.add(ptr_event);
                break;
        }

        mouse_motion.update(ptr_event);
    }

    void InputStay::reset()
    {
        keyboard.reset();
        window.reset();
        mouse_motion.reset();
    }
}
