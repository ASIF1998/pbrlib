#include <pbrlib/exceptions.hpp>

#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/vec2.hpp>

#include <pbrlib/utils/combine_hash.hpp>

#include <cmath>
#include <algorithm>

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr Vec3<T>::Vec3(T xyz) noexcept :
        x(xyz),
        y(xyz),
        z(xyz)
    {}

    template<MathArithmetic T>
    inline constexpr Vec3<T>::Vec3(T x, T y, T z) noexcept :
        x(x),
        y(y),
        z(z)
    {}

    template<MathArithmetic T>
    inline constexpr Vec3<T>::Vec3(const Vec2<T>& vec, T z) noexcept :
        x(vec.x),
        y(vec.y),
        z(z)
    {}

    template<MathArithmetic T>
    inline constexpr Vec3<T>::Vec3(const Vec4<T>& vec) noexcept :
        x (vec.x),
        y (vec.y),
        z (vec.z)
    { }

    template<MathArithmetic T>
    inline constexpr bool Vec3<T>::operator == (const Vec3<T>& v) const noexcept
    {
        if constexpr (std::is_floating_point<T>::value)
        {
            constexpr auto eps = static_cast<T>(0.0001);
            return
                    std::abs(x - v.x) < eps
                &&  std::abs(y - v.y) < eps
                &&  std::abs(z - v.z) < eps;
        }

        return x == v.x && y == v.y && z == v.z;
    }

    template<MathArithmetic T>
    inline constexpr bool Vec3<T>::operator != (const Vec3<T>& v) const noexcept
    {
        return !(*this == v);
    }

    template<MathArithmetic T>
    inline constexpr Vec3<T> Vec3<T>::operator + (const Vec3<T>& v) const noexcept
    {
        return Vec3<T>(x + v.x, y + v.y, z + v.z);
    }

    template<MathArithmetic T>
    inline constexpr Vec3<T> Vec3<T>::operator - (const Vec3<T>& v) const noexcept
    {
        return Vec3<T>(x - v.x, y - v.y, z - v.z);
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

    template<MathArithmetic T>
    inline constexpr Vec3<T>& Vec3<T>::operator += (const Vec3<T>& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Vec3<T>& Vec3<T>::operator -= (const Vec3<T>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Vec3<T>& Vec3<T>::operator *= (T s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;

        return *this;
    }

    template<MathArithmetic T>
    T& Vec3<T>::operator [] (size_t i) noexcept
    {
        return xyz[i];
    }

    template<MathArithmetic T>
    inline constexpr T Vec3<T>::operator [] (size_t i) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct field access for constexpr evaluation
        return (i == 0) ? x : ((i == 1) ? y : z);
    }

    template<MathArithmetic T>
    inline constexpr T Vec3<T>::lengthSquared() const noexcept
    {
        return x * x + y * y + z * z;
    }

    template<MathArithmetic T>
    inline T Vec3<T>::length() const noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    template<MathArithmetic T>
    void Vec3<T>::normalize()
    {
        auto l = length();

        if (l == static_cast<T>(0)) [[unlikely]]
            throw exception::MathError("[vec3] failed normalize");

        x /= l;
        y /= l;
        z /= l;
    }

    template<MathArithmetic T>
    inline constexpr T dot(const Vec3<T> v1, const Vec3<T>& v2) noexcept
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<MathArithmetic T>
    inline constexpr Vec3<T> cross(const Vec3<T> v1, const Vec3<T>& v2) noexcept
    {
        return Vec3<T>
        (
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    template<MathArithmetic T>
    Vec3<T> normalize(const Vec3<T>& v)
    {
        auto l = v.length();

        if (l == static_cast<T>(0)) [[unlikely]]
            throw exception::MathError("[vec3] failed normalize");

        return Vec3<T>(v.x / l, v.y / l, v.z / l);
    }

    template<MathArithmetic T>
    inline Vec3<T> round(const Vec3<T>& v) noexcept
    {
        return Vec3<T>(std::round(v.x), std::round(v.y), std::round(v.z));
    }

    template<MathArithmetic T>
    inline Vec3<T> clamp(const Vec3<T>& x, T min_val, T max_val) noexcept
    {
        return Vec3<T> (
            std::clamp(x.x, min_val, max_val),
            std::clamp(x.y, min_val, max_val),
            std::clamp(x.z, min_val, max_val)
        );
    }

    template<MathArithmetic T>
    inline Vec3<T> clamp (
        const Vec3<T>& x,
        const Vec3<T>& min_val,
        const Vec3<T>& max_val
    ) noexcept
    {
        return Vec3<T> (
            std::clamp(x.x, min_val.x, max_val.x),
            std::clamp(x.y, min_val.y, max_val.y),
            std::clamp(x.z, min_val.z, max_val.z)
        );
    }

    template<MathArithmetic T>
    inline Vec3<T> abs(const Vec3<T>& v) noexcept
    {
        return Vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
    }
}

namespace std
{
    template<pbrlib::math::MathArithmetic T>
    constexpr auto formatter<pbrlib::math::Vec3<T>>::parse(format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    template<pbrlib::math::MathArithmetic T>
    auto formatter<pbrlib::math::Vec3<T>>::format (
        const pbrlib::math::Vec3<T>&    vec,
        format_context&                 ctx
    ) const
    {
        return format_to(ctx.out(), "vec3[{}, {}, {}]", vec.x, vec.y, vec.z);
    }

    template<pbrlib::math::MathArithmetic T>
    inline constexpr size_t hash<pbrlib::math::Vec3<T>>::operator () (const pbrlib::math::Vec3<T>& vec) const noexcept
    {
        size_t hash_value = 0;
        pbrlib::utils::combineHash(hash_value, vec.x);
        pbrlib::utils::combineHash(hash_value, vec.y);
        pbrlib::utils::combineHash(hash_value, vec.z);

        return hash_value;
    }
}
