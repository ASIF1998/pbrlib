#include <pbrlib/exceptions.hpp>
#include <cmath>
#include <algorithm>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    inline constexpr Vec3<Type>::Vec3(Type xyz) noexcept :
        x(xyz),
        y(xyz),
        z(xyz)
    {}

    template<MathArithmetic Type>
    inline constexpr Vec3<Type>::Vec3(Type x, Type y, Type z) noexcept :
        x(x),
        y(y),
        z(z)
    {}

    template<MathArithmetic Type>
    inline constexpr Vec3<Type>::Vec3(const Vec2<Type>& vec2, Type z) noexcept :
        x(vec2.x),
        y(vec2.y),
        z(z)
    {}

    template<MathArithmetic Type>
    inline constexpr bool Vec3<Type>::operator == (const Vec3<Type>& v) const noexcept
    {
        return x == v.x && y == v.y && z == v.z;
    }

    template<MathArithmetic Type>
    inline constexpr bool Vec3<Type>::operator != (const Vec3<Type>& v) const noexcept
    {
        return x != v.x || y != v.y || z != v.z;
    }

    template<MathArithmetic Type>
    inline constexpr Vec3<Type> Vec3<Type>::operator + (const Vec3<Type>& v) const noexcept
    {
        return Vec3<Type>(x + v.x, y + v.y, z + v.z);
    }

    template<MathArithmetic Type>
    inline constexpr Vec3<Type> Vec3<Type>::operator - (const Vec3<Type>& v) const noexcept
    {
        return Vec3<Type>(x - v.x, y - v.y, z - v.z);
    }

    template<MathArithmetic T>
    inline constexpr Vec3<T> operator * (const Vec3<T>& v, T s)
    {
        return Vec3<T>(v.x * s, v.y * s, v.z * s);
    }

    template<MathArithmetic T>
    inline constexpr Vec3<T> operator * (T s, const Vec3<T>& v)
    {
        return Vec3<T>(v.x * s, v.y * s, v.z * s);
    }

    template<MathArithmetic Type>
    inline constexpr Vec3<Type>& Vec3<Type>::operator += (const Vec3<Type>& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Vec3<Type>& Vec3<Type>::operator -= (const Vec3<Type>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Vec3<Type>& Vec3<Type>::operator *= (Type s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;

        return *this;
    }

    template<MathArithmetic Type>
    Type& Vec3<Type>::operator [] (size_t i) noexcept
    {
        return xyz[i];
    }

    template<MathArithmetic Type>
    inline constexpr Type Vec3<Type>::operator [] (size_t i) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct field access for constexpr evaluation
        return (i == 0) ? x : ((i == 1) ? y : z);
    }

    template<MathArithmetic Type>
    inline constexpr Type Vec3<Type>::lengthSquared() const noexcept
    {
        return x * x + y * y + z * z;
    }

    template<MathArithmetic Type>
    inline Type Vec3<Type>::length() const noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    template<MathArithmetic Type>
    void Vec3<Type>::normalize()
    {
        auto l = length();
        
        if (l == static_cast<Type>(0)) [[unlikely]]
            throw exception::MathError("[vec3] failed normalize");

        x /= l;
        y /= l;
        z /= l;
    }

    template<MathArithmetic Type>
    inline constexpr Type dot(const Vec3<Type> v1, const Vec3<Type>& v2) noexcept
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<MathArithmetic Type>
    inline constexpr Vec3<Type> cross(const Vec3<Type> v1, const Vec3<Type>& v2) noexcept
    {
        return Vec3<Type>
        (
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    template<MathArithmetic Type>
    Vec3<Type> normalize(const Vec3<Type>& v)
    {
        auto l = v.length();
        
        if (l == static_cast<Type>(0)) [[unlikely]]
            throw exception::MathError("[vec3] failed normalize");

        return Vec3<Type>(v.x / l, v.y / l, v.z / l);
    }

    template<MathArithmetic Type>
    inline Vec3<Type> round(const Vec3<Type>& v) noexcept
    {
        return Vec3<Type>(std::round(v.x), std::round(v.y), std::round(v.z));
    }

    template<MathArithmetic Type>
    inline Vec3<Type> clamp(const Vec3<Type>& x, Type min_val, Type max_val) noexcept
    {
        return Vec3<Type> (
            std::clamp(x.x, min_val, max_val),
            std::clamp(x.y, min_val, max_val),
            std::clamp(x.z, min_val, max_val)
        );
    }

    template<MathArithmetic Type>
    inline Vec3<Type> clamp (
        const Vec3<Type>& x, 
        const Vec3<Type>& min_val, 
        const Vec3<Type>& max_val
    ) noexcept
    {
        return Vec3<Type> (
            std::clamp(x.x, min_val.x, max_val.x),
            std::clamp(x.y, min_val.y, max_val.y),
            std::clamp(x.z, min_val.z, max_val.z)
        );
    }

    template<MathArithmetic Type>
    inline Vec3<Type> abs(const Vec3<Type>& v) noexcept
    {
        return Vec3<Type>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
    }
}
