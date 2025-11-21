#pragma once

#include <concepts>

namespace pbrlib::math
{
    template<typename T>
    concept MathArithmetic = requires(T a, T b) 
    {
        { a + b }   -> std::convertible_to<T>;
        { a - b }   -> std::convertible_to<T>;
        { a * b }   -> std::convertible_to<T>;
        { a / b }   -> std::convertible_to<T>;
        { a == b }  -> std::convertible_to<bool>;
        { a != b }  -> std::convertible_to<bool>;
        { -a }      -> std::convertible_to<T>;
        { +a }      -> std::convertible_to<T>;
    }; 
}