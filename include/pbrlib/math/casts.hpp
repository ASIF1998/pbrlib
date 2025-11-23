#pragma once

#include <concepts>

#include <pbrlib/math/quat.hpp>
#include <pbrlib/math/matrix4x4.hpp>

namespace pbrlib::math
{
    mat4 toMatrix(const quat& q);
    quat toQuaternion(const mat4& mat);

    template<std::floating_point T>
    inline constexpr T toRadians(T degrees);

    template<std::floating_point T>
    inline constexpr T toDegrees(T radians);
}

#include <pbrlib/math/casts.inl>