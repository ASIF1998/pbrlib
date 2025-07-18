#pragma once

#include <concepts>

namespace pbrlib::math
{
    template <typename T>
    T lepr(const T& v_min, const T& v_max, std::floating_point auto t)
    {
        using U = decltype(t);
        return v_min * (static_cast<U>(1.0) - t) + v_max * t;
    }
}