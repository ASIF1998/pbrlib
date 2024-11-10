#pragma once

#include <SDL3/SDL_events.h>

#include <backend/utils/enum_cast.hpp>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/input/key_code.hpp>

#include <array>
#include <unordered_map>

namespace pbrlib
{
	class KeyboardStay
	{
		friend struct InputStay;

		enum class KeyStay
		{
			Down,
			Up, 
			None
		};

		void add(const SDL_Event* ptr_event);
		void reset();

	public:
		bool isDown(Keycode key_code) 	const;
		bool isUp(Keycode key_code) 	const;

	private:
		std::array<KeyStay, utils::enumCast(Keycode::Count)> _pressed_map;

		static const std::unordered_map<SDL_Keycode, Keycode> SDLtoPBRLibKeycode;
	};
}

namespace pbrlib
{
	class WindowStay
	{
		friend struct InputStay;

		void add(const SDL_Event* ptr_event);
		void reset();

	public:
		bool isClsoe() const noexcept;

	private:
		bool _is_close = false;
	};
}

namespace pbrlib
{
	enum class MouseButton
	{
		Left, 
		Middle,
		Right
	};

	class MouseButtonsStay
	{
		friend struct InputStay;

		void add(const SDL_Event* ptr_event);
		void reset();

		enum class ButtonStay
		{
			Down,
			Up,
			None
		};

	public:
		bool isDown(MouseButton button) const;
		bool isUp(MouseButton button)	const;

	private:
		std::array<ButtonStay, 3> _pressed_map;
	};
}

namespace pbrlib
{
	class MouseMotionStay
	{
		friend struct InputStay;

		void update(const SDL_Event* ptr_event);
		void reset();

	public:
		math::vec2 getCurrentCoord()	const;
		math::vec2 getRelativeMotion() 	const;

		bool isMotion() const noexcept;

	private:
		math::vec2 _current_coord;
		math::vec2 _relative_motion;

		bool _is_motion = false;
	};
}

namespace pbrlib
{
	struct InputStay
	{
	private:
		friend class SceneView;

		void add(const SDL_Event* ptr_event);
		void reset();

	public:
		KeyboardStay 		keyboard;
		WindowStay			window;
		MouseButtonsStay	mouse_buttons;
		MouseMotionStay		mouse_motion;
	};
}