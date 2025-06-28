#include <pbrlib/exceptions.hpp>

#include <format>

namespace pbrlib::math
{
    template<typename Type>
    inline constexpr Vec2<Type>::Vec2() noexcept :
        x{static_cast<Type>(0)},
        y{static_cast<Type>(0)}
    {}

    template<typename Type>
    inline constexpr Vec2<Type>::Vec2(Type xy) noexcept :
        x{xy},
        y{xy}
    {}

    template<typename Type>
    inline constexpr Vec2<Type>::Vec2(Type x, Type y) noexcept :
        x{x},
        y{y}
    {}

    template<typename Type>
    inline bool Vec2<Type>::operator == (const Vec2<Type>& v) const noexcept
    {
        return x == v.x && y == v.y;
    }

    template<typename Type>
    inline bool Vec2<Type>::operator != (const Vec2<Type>& v) const noexcept
    {
        return x != v.x || y != v.y;
    }

    template<typename Type>
    inline Vec2<Type> Vec2<Type>::operator + (const Vec2<Type>& v) const noexcept
    {
        return Vec2<Type>(x + v.x, y + v.y);
    }

    template<typename Type>
    inline Vec2<Type> Vec2<Type>::operator - (const Vec2<Type>& v) const noexcept
    {
        return Vec2<Type>(x - v.x, y - v.y);
    }

    template<typename Type>
    inline Vec2<Type> Vec2<Type>::operator * (Type s) const noexcept
    {
        return Vec2<Type>(x * s, y * s);
    }

    template<typename Type>
    inline Vec2<Type>& Vec2<Type>::operator += (const Vec2<Type>& v) noexcept
    {
        x += v.x;
        y += v.y;

        return *this;
    }

    template<typename Type>
    inline Vec2<Type>& Vec2<Type>::operator -= (const Vec2<Type>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        
        return *this;
    }

    template<typename Type>
    inline Vec2<Type>& Vec2<Type>::operator *= (Type s) noexcept
    {
        x *= s;
        y *= s;

        return *this;
    }

    template<typename Type>
    inline Type& Vec2<Type>::operator [] (size_t i) noexcept
    {
        return xy[i];
    }

    template<typename Type>
    inline Type Vec2<Type>::operator [] (size_t i) const noexcept
    {
        return xy[i];
    }

    template<typename Type>
    inline Type Vec2<Type>::lengthSquared() const noexcept
    {
        return x * x + y * y;
    }

    template<typename Type>
    inline Type Vec2<Type>::length() const noexcept
    {
        return sqrt(x * x + y * y);
    }

    template<typename Type>
    void Vec2<Type>::normalize()
    {
        auto l = length();

        if (l == static_cast<Type>(0)) [[unlikely]]
            throw exception::MathError("[vec2] failed normalize");

        x /= l;
        y /= l;
    }

    template<typename Type>
    Vec2<Type> normalize(const Vec2<Type>& v)
    {
        auto l = v.length();

        if (l == static_cast<Type>(0)) [[unlikely]]
            throw exception::MathError("[math::vec2] failed normalize");

        return Vec2<Type>(v.x / l, v.y / l);
    }
}
