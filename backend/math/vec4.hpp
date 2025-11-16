#pragma once

#include "concepts.hpp"
#include "vec3.hpp"
#include "vec2.hpp"

namespace pbrlib::math
{
    template<MathArithmetic Type>
    struct Vec4 final
    {
        inline constexpr Vec4(Type xyzw = static_cast<Type>(0))                         noexcept;
        inline constexpr Vec4(Type x, Type y, Type z, Type w)                           noexcept;
        inline constexpr Vec4(const Vec3<Type>& vec3, Type w = static_cast<Type>(0))    noexcept;
        inline constexpr Vec4(const Vec2<Type>& v1, const Vec2<Type>& v2)               noexcept;

        inline constexpr bool operator == (const Vec4& v) const noexcept;
        inline constexpr bool operator != (const Vec4& v) const noexcept;

        inline constexpr Vec4 operator + (const Vec4& v)  const noexcept;
        inline constexpr Vec4 operator - (const Vec4& v)  const noexcept;

        inline constexpr Vec4& operator += (const Vec4& v)  noexcept;
        inline constexpr Vec4& operator -= (const Vec4& v)  noexcept;
        inline constexpr Vec4& operator *= (Type s)         noexcept;

        inline Type&            operator [] (size_t i) noexcept;
        inline constexpr Type   operator [] (size_t i) const noexcept;

        inline constexpr Type   lengthSquared() const noexcept;
        inline Type             length()        const noexcept;

        inline void normalize();

        union
        {
            struct
            {
                Type x;
                Type y;
                Type z;
                Type w;
            };

            struct
            {
                Type r;
                Type g;
                Type b;
                Type a;
            };

            Type xyzw[4];
            Type rgba[4];
        };
    };

    template<MathArithmetic Type>
    inline constexpr Type dot(const Vec4<Type> v1, const Vec4<Type>& v2) noexcept;

    template<MathArithmetic Type>
    inline Vec4<Type> normalize(const Vec4<Type>& v);

    template<MathArithmetic Type>
    inline Vec4<Type> round(const Vec4<Type>& v) noexcept;

    template<MathArithmetic Type>
    inline Vec4<Type> clamp(const Vec4<Type>& x, Type min_val, Type max_val) noexcept;

    template<MathArithmetic Type>
    inline Vec4<Type> clamp (
        const Vec4<Type>& x, 
        const Vec4<Type>& min_val, 
        const Vec4<Type>& max_val
    ) noexcept;

    template<MathArithmetic Type>
    inline Vec4<Type> abs(const Vec4<Type>& v) noexcept;

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (const Vec4<T>& v, T s);

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (T s, const Vec4<T>& v);
}

#include <backend/math/vec4.inl>
