#include <pbrlib/exceptions.hpp>
#include <cmath>
#include <algorithm>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    inline constexpr Vec2<Type>::Vec2() noexcept :
        x(static_cast<Type>(0)),
        y(static_cast<Type>(0))
    {}

    template<MathArithmetic Type>
    inline constexpr Vec2<Type>::Vec2(Type xy) noexcept :
        x(xy),
        y(xy)
    {}

    template<MathArithmetic Type>
    inline constexpr Vec2<Type>::Vec2(Type x, Type y) noexcept :
        x(x),
        y(y)
    {}

    template<MathArithmetic Type>
    inline constexpr bool Vec2<Type>::operator == (const Vec2<Type>& v) const noexcept
    {
        return x == v.x && y == v.y;
    }

    template<MathArithmetic Type>
    inline constexpr bool Vec2<Type>::operator != (const Vec2<Type>& v) const noexcept
    {
        return x != v.x || y != v.y;
    }

    template<MathArithmetic Type>
    inline constexpr Vec2<Type> Vec2<Type>::operator + (const Vec2<Type>& v) const noexcept
    {
        return Vec2<Type>(x + v.x, y + v.y);
    }

    template<MathArithmetic Type>
    inline constexpr Vec2<Type> Vec2<Type>::operator - (const Vec2<Type>& v) const noexcept
    {
        return Vec2<Type>(x - v.x, y - v.y);
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

    template<MathArithmetic Type>
    inline constexpr Vec2<Type>& Vec2<Type>::operator += (const Vec2<Type>& v) noexcept
    {
        x += v.x;
        y += v.y;

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Vec2<Type>& Vec2<Type>::operator -= (const Vec2<Type>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        
        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Vec2<Type>& Vec2<Type>::operator *= (Type s) noexcept
    {
        x *= s;
        y *= s;

        return *this;
    }

    template<MathArithmetic Type>
    inline Type& Vec2<Type>::operator [] (size_t i) noexcept
    {
        return xy[i];
    }

    template<MathArithmetic Type>
    inline constexpr Type Vec2<Type>::operator [] (size_t i) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct field access for constexpr evaluation
        return (i == 0) ? x : y;
    }

    template<MathArithmetic Type>
    inline constexpr Type Vec2<Type>::lengthSquared() const noexcept
    {
        return x * x + y * y;
    }

    template<MathArithmetic Type>
    inline Type Vec2<Type>::length() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    template<MathArithmetic Type>
    void Vec2<Type>::normalize()
    {
        auto l = length();

        if (l == static_cast<Type>(0)) [[unlikely]]
            throw exception::MathError("[vec2] failed normalize");

        x /= l;
        y /= l;
    }

    template<MathArithmetic Type>
    Vec2<Type> normalize(const Vec2<Type>& v)
    {
        auto l = v.length();

        if (l == static_cast<Type>(0)) [[unlikely]]
            throw exception::MathError("[math::vec2] failed normalize");

        return Vec2<Type>(v.x / l, v.y / l);
    }

    template<MathArithmetic Type>
    inline Vec2<Type> round(const Vec2<Type>& v) noexcept
    {
        return Vec2<Type>(std::round(v.x), std::round(v.y));
    }

    template<MathArithmetic Type>
    inline Vec2<Type> clamp(const Vec2<Type>& x, Type min_val, Type max_val) noexcept
    {
        return Vec2<Type> (
            std::clamp(x.x, min_val, max_val),
            std::clamp(x.y, min_val, max_val)
        );
    }

    template<MathArithmetic Type>
    inline Vec2<Type> clamp (
        const Vec2<Type>& x, 
        const Vec2<Type>& min_val, 
        const Vec2<Type>& max_val
    ) noexcept
    {
        return Vec2<Type> (
            std::clamp(x.x, min_val.x, max_val.x),
            std::clamp(x.y, min_val.y, max_val.y)
        );
    }

    template<MathArithmetic Type>
    inline Vec2<Type> abs(const Vec2<Type>& v) noexcept
    {
        return Vec2<Type>(std::abs(v.x), std::abs(v.y));
    }
}
