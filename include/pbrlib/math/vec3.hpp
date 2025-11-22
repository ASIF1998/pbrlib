#pragma once

#include <pbrlib/math/concepts.hpp>

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec4;

    template<MathArithmetic T>
    struct Vec2;
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec3 final
    {
        inline constexpr Vec3(T xyz = static_cast<T>(0))                    noexcept;
        inline constexpr Vec3(T x, T y, T z = static_cast<T>(0))            noexcept;
        inline constexpr Vec3(const Vec2<T>& vec, T z = static_cast<T>(0))  noexcept;
        inline constexpr Vec3(const Vec4<T>& vec)                           noexcept;

        inline constexpr bool operator == (const Vec3& v) const noexcept;
        inline constexpr bool operator != (const Vec3& v) const noexcept;

        inline constexpr Vec3 operator + (const Vec3& v) const noexcept;
        inline constexpr Vec3 operator - (const Vec3& v) const noexcept;

        inline constexpr Vec3& operator += (const Vec3& v)  noexcept;
        inline constexpr Vec3& operator -= (const Vec3& v)  noexcept;
        inline constexpr Vec3& operator *= (T s)            noexcept;

        inline T&           operator [] (size_t i) noexcept;
        inline constexpr T  operator [] (size_t i) const noexcept;

        inline T             length()        const noexcept;
        inline constexpr T   lengthSquared() const noexcept;

        void normalize();

        union
        {
            struct
            {
                T x;
                T y;
                T z;
            };
            
            struct
            {
                T r;
                T g;
                T b;
            };

            T xyz[3];
            T rgb[3];
        };
    };

    template<MathArithmetic T>
    inline constexpr T dot(const Vec3<T> v1, const Vec3<T>& v2) noexcept;

    template<MathArithmetic T>
    inline constexpr Vec3<T> cross(const Vec3<T> v1, const Vec3<T>& v2) noexcept;

    template<MathArithmetic T>
    Vec3<T> normalize(const Vec3<T>& v);

    template<MathArithmetic T>
    inline Vec3<T> round(const Vec3<T>& v) noexcept;

    template<MathArithmetic T>
    inline Vec3<T> round(const Vec3<T>& v) noexcept;

    template<MathArithmetic T>
    inline Vec3<T> clamp(const Vec3<T>& x, T min_val, T max_val) noexcept;

    template<MathArithmetic T>
    inline Vec3<T> clamp (
        const Vec3<T>& x, 
        const Vec3<T>& min_val, 
        const Vec3<T>& max_val
    ) noexcept;

    template<MathArithmetic T>
    inline Vec3<T> abs(const Vec3<T>& v) noexcept;

    template<MathArithmetic T>
    inline constexpr Vec3<T> operator * (const Vec3<T>& v, T s);

    template<MathArithmetic T>
    inline constexpr Vec3<T> operator * (T s, const Vec3<T>& v);
}

namespace pbrlib::math
{
    using vec3      = Vec3<float>;
    using dvec3     = Vec3<double>;
    using ivec3     = Vec3<int32_t>;
    using uvec3     = Vec3<uint32_t>;
    using i16vec3   = Vec3<int16_t>;
    using u16vec3   = Vec3<uint16_t>;
    using i8vec3    = Vec3<int8_t>;
    using u8vec3    = Vec3<uint8_t>;
}

#include <pbrlib/math/vec3.inl>
