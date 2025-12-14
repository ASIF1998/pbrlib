#include <pbrlib/exceptions.hpp>

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>

#include <pbrlib/utils/combine_hash.hpp>

#include <cmath>
#include <algorithm>

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr Vec2<T>::Vec2() noexcept :
        x(static_cast<T>(0)),
        y(static_cast<T>(0))
    {}

    template<MathArithmetic T>
    inline constexpr Vec2<T>::Vec2(T xy) noexcept :
        x(xy),
        y(xy)
    {}

    template<MathArithmetic T>
    inline constexpr Vec2<T>::Vec2(T x, T y) noexcept :
        x(x),
        y(y)
    {}

    template<MathArithmetic T>
    inline constexpr Vec2<T>::Vec2(const Vec3<T>& vec) noexcept :
        x(vec.x),
        y(vec.y)
    { }
    
    template<MathArithmetic T>
    inline constexpr Vec2<T>::Vec2(const Vec4<T>& vec) noexcept :
        x(vec.x),
        y(vec.y)
    { }

    template<MathArithmetic T>
    inline constexpr bool Vec2<T>::operator == (const Vec2<T>& v) const noexcept
    {
        if constexpr (std::is_floating_point<T>::value)
        {
            constexpr auto eps = static_cast<T>(0.0001);
            return 
                    std::abs(x - v.x) < eps
                &&  std::abs(y - v.y) < eps;
        }

        return x == v.x && y == v.y;
    }

    template<MathArithmetic T>
    inline constexpr bool Vec2<T>::operator != (const Vec2<T>& v) const noexcept
    {
        return !(*this == v);
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T> Vec2<T>::operator + (const Vec2<T>& v) const noexcept
    {
        return Vec2<T>(x + v.x, y + v.y);
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T> Vec2<T>::operator - (const Vec2<T>& v) const noexcept
    {
        return Vec2<T>(x - v.x, y - v.y);
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T> operator * (const Vec2<T>& v, T s)
    {
        return Vec2<T>(v.x * s, v.y * s);
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T> operator * (T s, const Vec2<T>& v)
    {
        return Vec2<T>(v.x * s, v.y * s);
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T>& Vec2<T>::operator += (const Vec2<T>& v) noexcept
    {
        x += v.x;
        y += v.y;

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T>& Vec2<T>::operator -= (const Vec2<T>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        
        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T>& Vec2<T>::operator *= (T s) noexcept
    {
        x *= s;
        y *= s;

        return *this;
    }

    template<MathArithmetic T>
    inline T& Vec2<T>::operator [] (size_t i) noexcept
    {
        return xy[i];
    }

    template<MathArithmetic T>
    inline constexpr T Vec2<T>::operator [] (size_t i) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct field access for constexpr evaluation
        return (i == 0) ? x : y;
    }

    template<MathArithmetic T>
    inline constexpr T Vec2<T>::lengthSquared() const noexcept
    {
        return x * x + y * y;
    }

    template<MathArithmetic T>
    inline T Vec2<T>::length() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    template<MathArithmetic T>
    void Vec2<T>::normalize()
    {
        auto l = length();

        if (l == static_cast<T>(0)) [[unlikely]]
            throw exception::MathError("[vec2] failed normalize");

        x /= l;
        y /= l;
    }

    template<MathArithmetic T>
    Vec2<T> normalize(const Vec2<T>& v)
    {
        auto l = v.length();

        if (l == static_cast<T>(0)) [[unlikely]]
            throw exception::MathError("[math::vec2] failed normalize");

        return Vec2<T>(v.x / l, v.y / l);
    }

    template<MathArithmetic T>
    inline Vec2<T> round(const Vec2<T>& v) noexcept
    {
        return Vec2<T>(std::round(v.x), std::round(v.y));
    }

    template<MathArithmetic T>
    inline Vec2<T> clamp(const Vec2<T>& x, T min_val, T max_val) noexcept
    {
        return Vec2<T> (
            std::clamp(x.x, min_val, max_val),
            std::clamp(x.y, min_val, max_val)
        );
    }

    template<MathArithmetic T>
    inline Vec2<T> clamp (
        const Vec2<T>& x, 
        const Vec2<T>& min_val, 
        const Vec2<T>& max_val
    ) noexcept
    {
        return Vec2<T> (
            std::clamp(x.x, min_val.x, max_val.x),
            std::clamp(x.y, min_val.y, max_val.y)
        );
    }

    template<MathArithmetic T>
    inline Vec2<T> abs(const Vec2<T>& v) noexcept
    {
        return Vec2<T>(std::abs(v.x), std::abs(v.y));
    }
}

namespace std
{
    template<pbrlib::math::MathArithmetic T>
    constexpr auto formatter<pbrlib::math::Vec2<T>>::parse(format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    template<pbrlib::math::MathArithmetic T>
    auto formatter<pbrlib::math::Vec2<T>>::format (
        const pbrlib::math::Vec2<T>&    vec, 
        format_context&                 ctx
    ) const
    {
        return format_to(ctx.out(), "vec2[{}, {}]", vec.x, vec.y);
    }

    template<pbrlib::math::MathArithmetic T>
    inline constexpr size_t hash<pbrlib::math::Vec2<T>>::operator () (const pbrlib::math::Vec2<T>& vec) const noexcept
    {
        size_t hash_value = 0;
        pbrlib::combineHash(hash_value, vec.x);
        pbrlib::combineHash(hash_value, vec.y);

        return hash_value;
    }
}