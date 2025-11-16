#pragma once

#include <backend/math/concepts.hpp>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    struct Vec2 final
    {
        inline constexpr Vec2()                 noexcept;
        inline constexpr Vec2(Type xy)          noexcept;
        inline constexpr Vec2(Type x, Type y)   noexcept;

        inline constexpr bool operator == (const Vec2& v) const noexcept;
        inline constexpr bool operator != (const Vec2& v) const noexcept;
        
        inline constexpr Vec2 operator + (const Vec2& v)  const noexcept;
        inline constexpr Vec2 operator - (const Vec2& v)  const noexcept;

        inline constexpr Vec2& operator += (const Vec2& v)  noexcept;
        inline constexpr Vec2& operator -= (const Vec2& v)  noexcept;
        inline constexpr Vec2& operator *= (Type s)         noexcept;

        inline Type&            operator [] (size_t i) noexcept;
        inline constexpr Type   operator [] (size_t i) const noexcept;

        inline constexpr Type   lengthSquared()   const noexcept;
        inline Type             length()          const noexcept;

        void normalize();

        union
        {
            struct
            {
                Type x;
                Type y;
            };
            
            struct
            {
                Type r;
                Type g;
            };

            Type xy[2];
            Type rg[2];
        };
    };

    template<MathArithmetic Type>
    Vec2<Type> normalize(const Vec2<Type>& v);

    template<MathArithmetic Type>
    inline Vec2<Type> round(const Vec2<Type>& v) noexcept;

    template<MathArithmetic T>
    inline constexpr Vec2<T> operator * (const Vec2<T>& v, T s);

    template<MathArithmetic T>
    inline constexpr Vec2<T> operator * (T s, const Vec2<T>& v);
}

#include <backend/math/vec2.inl>
