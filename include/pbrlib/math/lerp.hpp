#pragma once

#include <concepts>

namespace pbrlib::math
{
    template <typename T, std::floating_point U>
    constexpr T lerp(const T& v_min, const T& v_max, U t)
    {
        return v_min * (static_cast<U>(1.0) - t) + v_max * t;
    }

    template <typename T, std::floating_point U>
    constexpr T slerp(const T& v_min, const T& v_max, U t)
    {
        U cos_theta = dot(v_min, v_max);

        if (cos_theta > static_cast<U>(0.995)) [[unlikely]] 
            return pbrlib::math::lerp(v_min, v_max, t);

        T v = v_max;

        if (cos_theta < static_cast<U>(0u)) [[unlikely]] 
        {
            v           *=  -1.0f;
            cos_theta   =   -cos_theta;
        }
        
        U theta = acos(cos_theta);

        return (v_min * sin((1.0f - t) * theta) + v * sin(t * theta)) / sin(theta);
    }
}