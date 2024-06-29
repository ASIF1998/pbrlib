#include <pbrlib/Input/InputStay.hpp>

#include <iostream>

#include <SDL3/SDL_keyboard.h>

#include "Private/InitSDLtoPBRLibKeycode.inl"

using namespace pbrlib::math;

namespace pbrlib
{
	void KeyboardStay::reset()
	{
		_pressed_map.fill(KeyboardStay::KeyStay::None);
	}

	void KeyboardStay::add(const SDL_Event* ptr_event)
	{
		if (auto keycode = SDLtoPBRLibKeycode.find(ptr_event->key.keysym.sym); keycode != std::end(SDLtoPBRLibKeycode))
		{
			auto keycode_index = utils::enumCast(keycode->second);

			if (ptr_event->type == SDL_EVENT_KEY_DOWN)
				_pressed_map[keycode_index] = KeyStay::Down;
			else if (ptr_event->type == SDL_EVENT_KEY_UP)
				_pressed_map[keycode_index] = KeyStay::Up;
		}
	}

	bool KeyboardStay::isDown(Keycode key_code) const
	{
		return _pressed_map[utils::enumCast(key_code)] == KeyStay::Down;
	}
	
    bool KeyboardStay::isUp(Keycode key_code) const
	{
		return _pressed_map[utils::enumCast(key_code)] == KeyStay::Up;
	}
}

namespace pbrlib
{
	void WindowStay::add(const SDL_Event* ptr_event)
	{
		if (ptr_event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			_is_close = true;
	}
	
	void WindowStay::reset()
	{
		_is_close = false;
	}

	bool WindowStay::isClsoe() const noexcept
	{
		return _is_close;
	}
}

namespace pbrlib
{
	void MouseButtonsStay::add(const SDL_Event* ptr_event)
	{
		size_t button_index = ptr_event->button.button;

		if (button_index < SDL_BUTTON_LEFT || button_index > SDL_BUTTON_RIGHT)
			return;

		--button_index;

		if (ptr_event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			_pressed_map[button_index] = ButtonStay::Down;
		else if (ptr_event->type == SDL_EVENT_MOUSE_BUTTON_UP)
			_pressed_map[button_index] = ButtonStay::Up;
	}
	
	void MouseButtonsStay::reset()
	{
		_pressed_map.fill(ButtonStay::None);	
	}

	bool MouseButtonsStay::isDown(MouseButton button) const
	{
		return _pressed_map[utils::enumCast(button)] == ButtonStay::Down;
	}
	
	bool MouseButtonsStay::isUp(MouseButton button) const
	{
		return _pressed_map[utils::enumCast(button)] == ButtonStay::Up;
	}
}

namespace pbrlib
{
	void MouseMotionStay::update(const SDL_Event* ptr_event)
	{
		if (ptr_event->type == SDL_EVENT_MOUSE_MOTION)
		{
			_current_coord.x = ptr_event->motion.x;
			_current_coord.y = ptr_event->motion.y;

			_relative_motion.x = ptr_event->motion.xrel;
			_relative_motion.y = ptr_event->motion.yrel;

			_is_motion = true;
		}
	}

	void MouseMotionStay::reset()
	{
		_is_motion = false;
	}

	Vec2<float> MouseMotionStay::getCurrentCoord() const
	{
		return _current_coord;
	}

	Vec2<float> MouseMotionStay::getRelativeMotion() const
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
	void InputStay::add(const SDL_Event* ptr_event)
	{
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
		mouse_buttons.reset();
		mouse_motion.reset();
	}
}