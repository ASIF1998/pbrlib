#pragma once

#include <memory>

namespace pbrlib
{
    template<typename T>
    inline constexpr void combineHash(size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}