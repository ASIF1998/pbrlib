#pragma once

#include <pbrlib/math/vec3.hpp>

namespace pbrlib::backend::vk::marker_colors
{
    constexpr pbrlib::math::vec3 generateColor(uint32_t val);
}

#include "generate_color.inl"