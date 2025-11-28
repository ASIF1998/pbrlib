#pragma once

#include <pbrlib/math/concepts.hpp>

#include <format>

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec4;

    template<MathArithmetic T>
    struct Vec3;
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec2 final
    {
        inline constexpr Vec2()                     noexcept;
        inline constexpr Vec2(T xy)                 noexcept;
        inline constexpr Vec2(T x, T y)             noexcept;
        inline constexpr Vec2(const Vec3<T>& vec)   noexcept;
        inline constexpr Vec2(const Vec4<T>& vec)   noexcept;

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

namespace std
{
    template<pbrlib::math::MathArithmetic T>
    struct formatter<pbrlib::math::Vec2<T>>
    {
        constexpr auto  parse(format_parse_context& ctx)                                const;
        auto            format(const pbrlib::math::Vec2<T>& vec, format_context& ctx)   const;
    };

    template<pbrlib::math::MathArithmetic T>
    struct hash<pbrlib::math::Vec2<T>> final
    {
        inline constexpr size_t operator () (const pbrlib::math::Vec2<T>& vec) const noexcept;
    };
}

namespace pbrlib::math
{
    using vec2      = Vec2<float>;
    using dvec2     = Vec2<double>;
    using ivec2     = Vec2<int32_t>;
    using uvec2     = Vec2<uint32_t>;
    using i16vec2   = Vec2<int16_t>;
    using u16vec2   = Vec2<uint16_t>;
    using i8vec2    = Vec2<int8_t>;
    using u8vec2    = Vec2<uint8_t>;
}

#include <pbrlib/math/vec2.inl>
