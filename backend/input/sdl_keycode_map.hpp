#pragma once

#include <unordered_map>

namespace pbrlib::backend::utils
{
    const std::unordered_map<SDL_Keycode, Keycode> sdl_to_pbrlib_keycode
    {
        {SDLK_A, Keycode::eA},
        {SDLK_B, Keycode::eB},
        {SDLK_C, Keycode::eC},
        {SDLK_D, Keycode::eD},
        {SDLK_E, Keycode::eE},
        {SDLK_F, Keycode::eF},
        {SDLK_G, Keycode::eG},
        {SDLK_H, Keycode::eH},
        {SDLK_I, Keycode::eI},
        {SDLK_J, Keycode::eJ},
        {SDLK_K, Keycode::eK},
        {SDLK_L, Keycode::eL},
        {SDLK_M, Keycode::eM},
        {SDLK_N, Keycode::eN},
        {SDLK_O, Keycode::eO},
        {SDLK_P, Keycode::eP},
        {SDLK_Q, Keycode::eQ},
        {SDLK_R, Keycode::eR},
        {SDLK_S, Keycode::eS},
        {SDLK_T, Keycode::eT},
        {SDLK_U, Keycode::eU},
        {SDLK_V, Keycode::eV},
        {SDLK_W, Keycode::eW},
        {SDLK_X, Keycode::eX},
        {SDLK_Y, Keycode::eY},
        {SDLK_Z, Keycode::eZ},

        {SDLK_0, Keycode::eN0},
        {SDLK_1, Keycode::eN1},
        {SDLK_2, Keycode::eN2},
        {SDLK_3, Keycode::eN3},
        {SDLK_4, Keycode::eN4},
        {SDLK_5, Keycode::eN5},
        {SDLK_6, Keycode::eN6},
        {SDLK_7, Keycode::eN7},
        {SDLK_8, Keycode::eN8},
        {SDLK_9, Keycode::eN9},

        {SDLK_F1, Keycode::eF1},
        {SDLK_F2, Keycode::eF2},
        {SDLK_F3, Keycode::eF3},
        {SDLK_F4, Keycode::eF4},
        {SDLK_F5, Keycode::eF5},
        {SDLK_F6, Keycode::eF6},
        {SDLK_F7, Keycode::eF7},
        {SDLK_F8, Keycode::eF8},
        {SDLK_F9, Keycode::eF9},
        {SDLK_F10, Keycode::eF10},
        {SDLK_F11, Keycode::eF11},
        {SDLK_F12, Keycode::eF12},

        {SDLK_SPACE, Keycode::eSpace},
        {SDLK_RETURN, Keycode::eEnter},
        {SDLK_LSHIFT, Keycode::eShiftLeft},
        {SDLK_RSHIFT, Keycode::eShiftRight},
        {SDLK_ESCAPE, Keycode::eEscape},

        {SDLK_TAB, Keycode::eTab},
        {SDLK_APOSTROPHE, Keycode::eQuote},
        {SDLK_SLASH, Keycode::eSlash},
        {SDLK_CAPSLOCK, Keycode::eCapslock},

        {SDLK_PLUS, Keycode::ePlus},
        {SDLK_MINUS, Keycode::eMinus},

        {SDLK_LALT, Keycode::eAltLeft},
        {SDLK_RALT, Keycode::eAltRight},

        {SDLK_LCTRL, Keycode::eCtrlLeft},
        {SDLK_RCTRL, Keycode::eCtrlRight},

        {SDLK_BACKSPACE, Keycode::eBackspace},

        {SDLK_UP, Keycode::eUp},
        {SDLK_DOWN, Keycode::eDown},
        {SDLK_RIGHT, Keycode::eRight},
        {SDLK_LEFT, Keycode::eLeft},
    };
}
