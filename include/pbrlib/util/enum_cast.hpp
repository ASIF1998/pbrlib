#pragma once

#include <type_traits>

namespace pbrlib::utils
{
    template<typename T>
    constexpr typename std::underlying_type<T>::type enumCast(T x)
    {
        return static_cast<typename std::underlying_type<T>::type>(x);
    }
}
