#include <pbrlib/exceptions.hpp>

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec2.hpp>

#include <pbrlib/utils/combine_hash.hpp>

#include <cmath>
#include <algorithm>

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr Vec4<T>::Vec4(T xyzw) noexcept :
        x(xyzw),
        y(xyzw),
        z(xyzw),
        w(xyzw)
    {}

    template<MathArithmetic T>
    inline constexpr Vec4<T>::Vec4(T x, T y, T z, T w) noexcept :
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    template<MathArithmetic T>
    inline constexpr Vec4<T>::Vec4(const Vec3<T>& vec3, T w) noexcept :
        x(vec3.x),
        y(vec3.y),
        z(vec3.z),
        w(w)
    {}

    template<MathArithmetic T>
    inline constexpr Vec4<T>::Vec4(const Vec2<T>& v1, const Vec2<T>& v2) noexcept :
        x(v1.x),
        y(v1.y),
        z(v2.x),
        w(v2.y)
    {}

    template<MathArithmetic T>
    inline constexpr bool Vec4<T>::operator == (const Vec4<T>& v) const noexcept
    {
        if constexpr (std::is_floating_point<T>::value)
        {
            constexpr auto eps = static_cast<T>(0.0001);
            return
                    std::abs(x - v.x) < eps
                &&  std::abs(y - v.y) < eps
                &&  std::abs(z - v.z) < eps
                &&  std::abs(w - v.w) < eps;
        }

        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    template<MathArithmetic T>
    inline constexpr bool Vec4<T>::operator != (const Vec4<T>& v) const noexcept
    {
        return !(*this == v);
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T> Vec4<T>::operator + (const Vec4<T>& v) const noexcept
    {
        return Vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T> Vec4<T>::operator - (const Vec4<T>& v) const noexcept
    {
        return Vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (const Vec4<T>& v, T s)
    {
        return Vec4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T> operator * (T s, const Vec4<T>& v)
    {
        return Vec4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T>& Vec4<T>::operator += (const Vec4<T>& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T>& Vec4<T>::operator -= (const Vec4<T>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T>& Vec4<T>::operator *= (T s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;

        return *this;
    }

    template<MathArithmetic T>
    inline T& Vec4<T>::operator [] (size_t i) noexcept
    {
        return xyzw[i];
    }

    template<MathArithmetic T>
    inline constexpr T Vec4<T>::operator [] (size_t i) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct field access for constexpr evaluation
        return (i == 0) ? x : ((i == 1) ? y : ((i == 2) ? z : w));
    }

    template<MathArithmetic T>
    inline constexpr T Vec4<T>::lengthSquared() const noexcept
    {
        return x * x + y * y + z * z + w * w;
    }

    template<MathArithmetic T>
    inline T Vec4<T>::length() const noexcept
    {
        return sqrt(lengthSquared());
    }

    template<MathArithmetic T>
    inline void Vec4<T>::normalize()
    {
        auto l = length();

        if (l == static_cast<T>(0)) [[unlikely]]
            throw exception::MathError("[vec4] failed normalize");

        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr T dot(const Vec4<T> v1, const Vec4<T>& v2) noexcept
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    template<MathArithmetic T>
    inline Vec4<T> normalize(const Vec4<T>& v)
    {
        Vec4<T> res (v);
        res.normalize();
        return res;
    }

    template<MathArithmetic T>
    inline Vec4<T> round(const Vec4<T>& v) noexcept
    {
        return Vec3<T>(std::round(v.x), std::round(v.y), std::round(v.z), std::round(v.w));
    }

    template<MathArithmetic T>
    inline Vec4<T> clamp(const Vec4<T>& x, T min_val, T max_val) noexcept
    {
        return Vec4<T> (
            std::clamp(x.x, min_val, max_val),
            std::clamp(x.y, min_val, max_val),
            std::clamp(x.z, min_val, max_val),
            std::clamp(x.w, min_val, max_val)
        );
    }

    template<MathArithmetic T>
    inline Vec4<T> clamp (
        const Vec4<T>& x,
        const Vec4<T>& min_val,
        const Vec4<T>& max_val
    ) noexcept
    {
        return Vec4<T> (
            std::clamp(x.x, min_val.x, max_val.x),
            std::clamp(x.y, min_val.y, max_val.y),
            std::clamp(x.z, min_val.z, max_val.z),
            std::clamp(x.w, min_val.w, max_val.w)
        );
    }

    template<MathArithmetic T>
    inline Vec4<T> abs(const Vec4<T>& v) noexcept
    {
        return Vec4<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
    }
}

namespace std
{
    template<pbrlib::math::MathArithmetic T>
    constexpr auto formatter<pbrlib::math::Vec4<T>>::parse(format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    template<pbrlib::math::MathArithmetic T>
    auto formatter<pbrlib::math::Vec4<T>>::format (
        const pbrlib::math::Vec4<T>&    vec,
        format_context&                 ctx
    ) const
    {
        return format_to(ctx.out(), "vec4[{}, {}, {}, {}]", vec.x, vec.y, vec.z, vec.w);
    }

    template<pbrlib::math::MathArithmetic T>
    inline constexpr size_t hash<pbrlib::math::Vec4<T>>::operator () (const pbrlib::math::Vec4<T>& vec) const noexcept
    {
        size_t hash_value = 0;
        pbrlib::utils::combineHash(hash_value, vec.x);
        pbrlib::utils::combineHash(hash_value, vec.y);
        pbrlib::utils::combineHash(hash_value, vec.z);
        pbrlib::utils::combineHash(hash_value, vec.w);

        return hash_value;
    }
}
