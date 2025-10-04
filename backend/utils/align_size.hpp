#pragma once

#include <concepts>

namespace pbrlib::backend::utils
{
    template<std::unsigned_integral T>
    constexpr T alignSize(T size, T alignment) noexcept 
    {
        return (size + alignment - 1) / alignment * alignment;
    }
}