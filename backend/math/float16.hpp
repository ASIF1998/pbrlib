#pragma once

#include <cstdint>
#include <bit>

namespace pbrlib::backend::math
{
    class float16_t final
    {
    public:
        inline constexpr float16_t(float value = 0.0f) noexcept;

        inline constexpr operator float() const noexcept;

        inline constexpr float16_t& operator += (float16_t v) noexcept;   

    private:
        uint16_t _bits = 0x0000u;
    };

    [[nodiscard]] inline constexpr float16_t    operator +  (float16_t a, float16_t b) noexcept;
    [[nodiscard]] inline constexpr float16_t    operator -  (float16_t a, float16_t b) noexcept;
    [[nodiscard]] inline constexpr float16_t    operator *  (float16_t a, float16_t b) noexcept;
    [[nodiscard]] inline constexpr float16_t    operator /  (float16_t a, float16_t b) noexcept;
    [[nodiscard]] inline constexpr bool         operator == (float16_t a, float16_t b) noexcept;
    [[nodiscard]] inline constexpr bool         operator != (float16_t a, float16_t b) noexcept;
    [[nodiscard]] inline constexpr bool         operator <  (float16_t a, float16_t b) noexcept;
    [[nodiscard]] inline constexpr bool         operator >  (float16_t a, float16_t b) noexcept;
}

#include <backend/math/float16.inl>