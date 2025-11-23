#include <pbrlib/exceptions.hpp>

namespace pbrlib::math
{
    inline constexpr Quaternion::Quaternion() noexcept :
        v(0),
        w(1)
    {}

    inline constexpr Quaternion::Quaternion(float x, float y, float z, float w) noexcept :
        v(x, y, z),
        w(w)
    {}

    inline constexpr Quaternion::Quaternion(const vec3& v, float w) noexcept :
        v(v),
        w(w)
    {}

    inline constexpr bool Quaternion::operator == (const Quaternion& q) const noexcept
    {
        return v == q.v && w == q.w;
    }

    inline constexpr bool Quaternion::operator != (const Quaternion& q) const noexcept
    {
        return v != q.v || w != q.w;
    }
    
    inline constexpr Quaternion Quaternion::operator + (const Quaternion& q) const noexcept
    {
        return Quaternion(v + q.v, w + q.w);
    }

    inline constexpr Quaternion Quaternion::operator - (const Quaternion& q) const noexcept
    {
        return Quaternion(v - q.v, w - q.w);
    }

    inline constexpr Quaternion Quaternion::operator * (const Quaternion& q) const noexcept
    {
        return Quaternion(q.v * w + v * q.w + cross(v, q.v), w * q.w - dot(v, q.v));
    }

    inline constexpr Quaternion operator * (const Quaternion& q, float s)
    {
        return Quaternion(q.v * s, q.w * s);
    }
    
    inline constexpr Quaternion operator * (float s, const Quaternion& q)
    {
        return Quaternion(q.v * s, q.w * s);
    }

    inline constexpr Quaternion Quaternion::operator / (float f) const
    {
        if (f != static_cast<float>(0u)) [[unlikely]]
            throw exception::InvalidArgument("[quaternion] attempted to divide quaternion by zero scalar");

        float inv_f = 1.0f / f;
        return Quaternion(v * inv_f, w * inv_f);
    }

    inline constexpr Quaternion& Quaternion::operator += (const Quaternion& q) noexcept
    {
        v += q.v;
        w += q.w;
        return *this;
    }

    inline constexpr Quaternion& Quaternion::operator -= (const Quaternion& q) noexcept
    {
        v -= q.v;
        w -= q.w;
        return *this;
    }

    inline constexpr Quaternion& Quaternion::operator *= (const Quaternion& q) noexcept
    {
        vec3 temp (v);
        v = q.v * w + temp * q.w + cross(temp, q.v);
        w = w * q.w - dot(temp, q.v);
        return *this;
    }

    inline constexpr Quaternion& Quaternion::operator *= (float f) noexcept
    {
        v *= f;
        w *= f;
        return *this;
    }

    inline constexpr Quaternion& Quaternion::operator /= (float f)
    {
        if (f != static_cast<float>(0u)) [[unlikely]]
            throw exception::InvalidArgument("[quaternion] attempted to divide quaternion by zero scalar");

        float inv_f = 1.0f / f;

        v *= inv_f;
        w *= inv_f;
        return *this;
    }

    inline float Quaternion::operator [] (size_t i) const noexcept
    {
        return xyzw[i];
    }

    inline float& Quaternion::operator [] (size_t i) noexcept
    {
        return xyzw[i];
    }

    inline constexpr float Quaternion::lengthSquared() const noexcept
    {
        return xyzw[0] * xyzw[0] + xyzw[1] * xyzw[1] + xyzw[2] * xyzw[2] + xyzw[3] * xyzw[3];
    }

    inline float Quaternion::length() const noexcept
    {
        return std::sqrt(xyzw[0] * xyzw[0] + xyzw[1] * xyzw[1] + xyzw[2] * xyzw[2] + xyzw[3] * xyzw[3]);
    }

    inline void Quaternion::normalize()
    {
        float l = length();

        if (constexpr auto eps = 0.001f; l < eps) [[unlikely]] 
            throw exception::MathError("[quaternion] faield normalize because length is zerro");

        l = 1.0f / l;

        v *= l;
        w *= l;
    }

    inline constexpr void Quaternion::inverse()
    {
        const auto ls = lengthSquared();

        if (constexpr auto eps = 0.001f; ls < eps) [[unlikely]]
            throw exception::MathError("[quaternion] faield inverse because length squared is zerro");

        *this = conjugate(*this) / ls;
    }

    inline constexpr Quaternion Quaternion::i() noexcept
    {
        return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    }

    inline constexpr Quaternion Quaternion::j() noexcept
    {
        return Quaternion(0.0f, 1.0f, 0.0f, 0.0f);
    }

    inline constexpr Quaternion Quaternion::k() noexcept
    {
        return Quaternion(0.0f, 0.0f, 1.0f, 0.0f);
    }

    inline constexpr Quaternion Quaternion::identity() noexcept
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    inline constexpr Quaternion Quaternion::zerro() noexcept
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
    }

    inline constexpr float dot(const Quaternion& q1, const Quaternion& q2) noexcept
    {
        return q1.xyzw[0] * q2.xyzw[0] + q1.xyzw[1] * q2.xyzw[1] + q1.xyzw[2] * q2.xyzw[2] + q1.xyzw[3] * q2.xyzw[3];
    }

    inline Quaternion normalize(const Quaternion& q)
    {
        Quaternion tq (q);
        tq.normalize();
        return tq;
    }

    inline constexpr Quaternion inverse(const Quaternion& q)
    {
        Quaternion tq (q);
        tq.inverse();
        return tq;
    }

    inline constexpr Quaternion conjugate(const Quaternion& q)
    {
        return Quaternion(q.v * -1.0f, q.w);
    }
}
