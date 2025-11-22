#pragma once

#include <pbrlib/math/concepts.hpp>

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec3;

    template<MathArithmetic T>
    struct Vec2;
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec4 final
    {
        inline constexpr Vec4(T xyzw = static_cast<T>(0))                   noexcept;
        inline constexpr Vec4(T x, T y, T z, T w)                           noexcept;
        inline constexpr Vec4(const Vec3<T>& vec3, T w = static_cast<T>(0)) noexcept;
        inline constexpr Vec4(const Vec2<T>& v1, const Vec2<T>& v2)         noexcept;

        inline constexpr bool operator == (const Vec4& v) const noexcept;
        inline constexpr bool operator != (const Vec4& v) const noexcept;

        inline constexpr Vec4 operator + (const Vec4& v)  const noexcept;
        inline constexpr Vec4 operator - (const Vec4& v)  const noexcept;

        inline constexpr Vec4& operator += (const Vec4& v)  noexcept;
        inline constexpr Vec4& operator -= (const Vec4& v)  noexcept;
        inline constexpr Vec4& operator *= (T s)            noexcept;

        inline T&            operator [] (size_t i) noexcept;
        inline constexpr T   operator [] (size_t i) const noexcept;

        inline T             length()        const noexcept;
        inline constexpr T   lengthSquared() const noexcept;

        inline void normalize();

        union
        {
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };

            struct
            {
                T r;
                T g;
                T b;
                T a;
            };

            T xyzw[4];
            T rgba[4];
        };
    };

    template<MathArithmetic T>
    inline constexpr T dot(const Vec4<T> v1, const Vec4<T>& v2) noexcept;

    template<MathArithmetic T>
    inline Vec4<T> normalize(const Vec4<T>& v);

    template<MathArithmetic T>
    inline Vec4<T> round(const Vec4<T>& v) noexcept;

    template<MathArithmetic T>
    inline Vec4<T> clamp(const Vec4<T>& x, T min_val, T max_val) noexcept;

    template<MathArithmetic T>
    inline Vec4<T> clamp (
        const Vec4<T>& x, 
        const Vec4<T>& min_val, 
        const Vec4<T>& max_val
    ) noexcept;

    template<MathArithmetic T>
    inline Vec4<T> abs(const Vec4<T>& v) noexcept;

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (const Vec4<T>& v, T s);

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (T s, const Vec4<T>& v);
}

namespace pbrlib::math
{
    using vec4      = Vec4<float>;
    using dvec4     = Vec4<double>;
    using ivec4     = Vec4<int32_t>;
    using uvec4     = Vec4<uint32_t>;
    using i16vec4   = Vec4<int16_t>;
    using u16vec4   = Vec4<uint16_t>;
    using i8vec4    = Vec4<int8_t>;
    using u8vec4    = Vec4<uint8_t>;
}

#include <pbrlib/math/vec4.inl>
