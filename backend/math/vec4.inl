#include <pbrlib/exceptions.hpp>
#include <cmath>
#include <algorithm>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    inline constexpr Vec4<Type>::Vec4(Type xyzw) noexcept :
        x(xyzw),
        y(xyzw),
        z(xyzw),
        w(xyzw)
    {}

    template<MathArithmetic Type>
    inline constexpr Vec4<Type>::Vec4(Type x, Type y, Type z, Type w) noexcept :
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    template<MathArithmetic Type>
    inline constexpr Vec4<Type>::Vec4(const Vec3<Type>& vec3, Type w) noexcept :
        x(vec3.x),
        y(vec3.y),
        z(vec3.z),
        w(w)
    {}

    template<MathArithmetic Type>
    inline constexpr Vec4<Type>::Vec4(const Vec2<Type>& v1, const Vec2<Type>& v2) noexcept :
        x(v1.x),
        y(v1.y),
        z(v2.x),
        w(v2.y)
    {}

    template<MathArithmetic Type>
    inline constexpr bool Vec4<Type>::operator == (const Vec4<Type>& v) const noexcept
    {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    template<MathArithmetic Type>
    inline constexpr bool Vec4<Type>::operator != (const Vec4<Type>& v) const noexcept
    {
        return x != v.x || y != v.y || z != v.z || w == v.w;
    }

    template<MathArithmetic Type>
    inline constexpr Vec4<Type> Vec4<Type>::operator + (const Vec4<Type>& v) const noexcept
    {
        return Vec4<Type>(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    template<MathArithmetic Type>
    inline constexpr Vec4<Type> Vec4<Type>::operator - (const Vec4<Type>& v) const noexcept
    {
        return Vec4<Type>(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (const Vec4<T>& v, T s)
    {
        return Vec4<T>(v.x * s, v.y * s, v.z * s, v.z * s);
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (T s, const Vec4<T>& v)
    {
        return Vec4<T>(v.x * s, v.y * s, v.z * s, v.z * s);
    }

    template<MathArithmetic Type>
    inline constexpr Vec4<Type>& Vec4<Type>::operator += (const Vec4<Type>& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Vec4<Type>& Vec4<Type>::operator -= (const Vec4<Type>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Vec4<Type>& Vec4<Type>::operator *= (Type s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;

        return *this;
    }

    template<MathArithmetic Type>
    inline Type& Vec4<Type>::operator [] (size_t i) noexcept
    {
        return xyzw[i];
    }

    template<MathArithmetic Type>
    inline constexpr Type Vec4<Type>::operator [] (size_t i) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct field access for constexpr evaluation
        return (i == 0) ? x : ((i == 1) ? y : ((i == 2) ? z : w));
    }

    template<MathArithmetic Type>
    inline constexpr Type Vec4<Type>::lengthSquared() const noexcept
    {
        return x * x + y * y + z * z + w * w;
    }

    template<MathArithmetic Type>
    inline Type Vec4<Type>::length() const noexcept
    {
        return sqrt(lengthSquared());
    }

    template<MathArithmetic Type>
    inline void Vec4<Type>::normalize()
    {
        auto l = length();

        if (l == static_cast<Type>(0)) [[unlikely]]
            throw exception::MathError("[vec4] failed normalize");

        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }
}

namespace pbrlib::math
{
    template<MathArithmetic Type>
    inline constexpr Type dot(const Vec4<Type> v1, const Vec4<Type>& v2) noexcept
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    template<MathArithmetic Type>
    inline Vec4<Type> normalize(const Vec4<Type>& v)
    {
        Vec4<Type> res (v);
        res.normalize();
        return res;
    }

    template<MathArithmetic Type>
    inline Vec4<Type> round(const Vec4<Type>& v) noexcept
    {
        return Vec3<Type>(std::round(v.x), std::round(v.y), std::round(v.z), std::round(v.w));
    }

    template<MathArithmetic Type>
    inline Vec4<Type> clamp(const Vec4<Type>& x, Type min_val, Type max_val) noexcept
    {
        return Vec4<Type> (
            std::clamp(x.x, min_val, max_val),
            std::clamp(x.y, min_val, max_val),
            std::clamp(x.z, min_val, max_val),
            std::clamp(x.w, min_val, max_val)
        );
    }

    template<MathArithmetic Type>
    inline Vec4<Type> clamp (
        const Vec4<Type>& x, 
        const Vec4<Type>& min_val, 
        const Vec4<Type>& max_val
    ) noexcept
    {
        return Vec4<Type> (
            std::clamp(x.x, min_val.x, max_val.x),
            std::clamp(x.y, min_val.y, max_val.y),
            std::clamp(x.z, min_val.z, max_val.z),
            std::clamp(x.w, min_val.w, max_val.w)
        );
    }

    template<MathArithmetic Type>
    inline Vec4<Type> abs(const Vec4<Type>& v) noexcept
    {
        return Vec4<Type>(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
    }
}
