#pragma once

#include "vec2.hpp"

namespace pbrlib::math
{
    template<MathArithmetic Type>
    struct Vec3 final
    {
        inline constexpr Vec3(Type xyz = static_cast<Type>(0))                          noexcept;
        inline constexpr Vec3(Type x, Type y, Type z = static_cast<Type>(0))            noexcept;
        inline constexpr Vec3(const Vec2<Type>& vec2, Type z = static_cast<Type>(0))    noexcept;

        inline constexpr bool operator == (const Vec3& v) const noexcept;
        inline constexpr bool operator != (const Vec3& v) const noexcept;

        inline constexpr Vec3 operator + (const Vec3& v) const noexcept;
        inline constexpr Vec3 operator - (const Vec3& v) const noexcept;

        inline constexpr Vec3& operator += (const Vec3& v)  noexcept;
        inline constexpr Vec3& operator -= (const Vec3& v)  noexcept;
        inline constexpr Vec3& operator *= (Type s)         noexcept;

        inline              Type&  operator [] (size_t i) noexcept;
        inline constexpr    Type   operator [] (size_t i) const noexcept;

        inline constexpr Type   lengthSquared() const noexcept;
        inline Type             length()        const noexcept;

        void normalize();

        union
        {
            struct
            {
                Type x;
                Type y;
                Type z;
            };
            
            struct
            {
                Type r;
                Type g;
                Type b;
            };

            Type xyz[3];
            Type rgb[3];
        };
    };

    template<MathArithmetic Type>
    inline constexpr Type dot(const Vec3<Type> v1, const Vec3<Type>& v2) noexcept;

    template<MathArithmetic Type>
    inline constexpr Vec3<Type> cross(const Vec3<Type> v1, const Vec3<Type>& v2) noexcept;

    template<MathArithmetic Type>
    Vec3<Type> normalize(const Vec3<Type>& v);

    template<MathArithmetic Type>
    inline Vec3<Type> round(const Vec3<Type>& v) noexcept;

    template<MathArithmetic T>
    inline constexpr Vec3<T> operator * (const Vec3<T>& v, T s);

    template<MathArithmetic T>
    inline constexpr Vec3<T> operator * (T s, const Vec3<T>& v);
}

#include <backend/math/vec3.inl>
