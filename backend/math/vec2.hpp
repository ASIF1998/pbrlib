#pragma once

#include <backend/math/concepts.hpp>

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec2 final
    {
        inline constexpr Vec2()         noexcept;
        inline constexpr Vec2(T xy)     noexcept;
        inline constexpr Vec2(T x, T y) noexcept;

        inline constexpr bool operator == (const Vec2& v) const noexcept;
        inline constexpr bool operator != (const Vec2& v) const noexcept;
        
        inline constexpr Vec2 operator + (const Vec2& v)  const noexcept;
        inline constexpr Vec2 operator - (const Vec2& v)  const noexcept;

        inline constexpr Vec2& operator += (const Vec2& v)  noexcept;
        inline constexpr Vec2& operator -= (const Vec2& v)  noexcept;
        inline constexpr Vec2& operator *= (T s)            noexcept;

        inline T&            operator [] (size_t i) noexcept;
        inline constexpr T   operator [] (size_t i) const noexcept;

        inline T             length()          const noexcept;
        inline constexpr T   lengthSquared()   const noexcept;

        void normalize();

        union
        {
            struct
            {
                T x;
                T y;
            };
            
            struct
            {
                T r;
                T g;
            };

            T xy[2];
            T rg[2];
        };
    };

    template<MathArithmetic T>
    Vec2<T> normalize(const Vec2<T>& v);

    template<MathArithmetic T>
    inline Vec2<T> round(const Vec2<T>& v) noexcept;

    template<MathArithmetic T>
    inline Vec2<T> clamp(const Vec2<T>& x, T min_val, T max_val) noexcept;

    template<MathArithmetic T>
    inline Vec2<T> clamp (
        const Vec2<T>& x, 
        const Vec2<T>& min_val, 
        const Vec2<T>& max_val
    ) noexcept;

    template<MathArithmetic T>
    inline Vec2<T> abs(const Vec2<T>& v) noexcept;

    template<MathArithmetic T>
    inline constexpr Vec2<T> operator * (const Vec2<T>& v, T s);

    template<MathArithmetic T>
    inline constexpr Vec2<T> operator * (T s, const Vec2<T>& v);
}

#include <backend/math/vec2.inl>
