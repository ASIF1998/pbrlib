//
//  quaternion.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 15/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../core.hpp"

using namespace std;

namespace pbrlib::math
{
    inline constexpr Quaternion::Quaternion() :
        w(1)
    {}

    inline constexpr Quaternion::Quaternion(float x, float y, float z, float w) :   
        v(x, y, z),
        w(w)
    {}

    inline constexpr Quaternion::Quaternion(const Vec3<float>& v, float w) :
        v(v),
        w(w)
    {}

    inline constexpr Quaternion::Quaternion(const __m128& m) noexcept :
        xyzw_simd(m)
    {}

    bool Quaternion::operator == (const Quaternion& q) const noexcept
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw_simd, q.xyzw_simd)) == 15;
    }


    bool Quaternion::operator != (const Quaternion& q) const noexcept
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw_simd, q.xyzw_simd)) != 15;
    }
    
    inline Quaternion Quaternion::operator + (const Quaternion& q) const noexcept
    {
        return Quaternion(_mm_add_ps(xyzw_simd, q.xyzw_simd));
    }

    inline Quaternion Quaternion::operator - (const Quaternion& q) const noexcept
    {
        return Quaternion(_mm_sub_ps(xyzw_simd, q.xyzw_simd));
    }

    inline Quaternion Quaternion::operator * (const Quaternion& q) const noexcept
    {
        return Quaternion(q.v * w + v * q.w + cross(v, q.v), w * q.w - dot(v, q.v));
    }

    inline Quaternion Quaternion::operator * (float f) const noexcept
    {
        return Quaternion(_mm_mul_ps(xyzw_simd, _mm_set1_ps(f)));
    }

    inline Quaternion Quaternion::operator / (float f) const noexcept
    {
        assert(f != static_cast<float>(0u));
        return Quaternion(_mm_mul_ps(xyzw_simd, _mm_set1_ps(1.0f / f)));
    }

    inline Quaternion& Quaternion::operator += (const Quaternion& q) noexcept
    {
        xyzw_simd = _mm_add_ps(xyzw_simd, q.xyzw_simd);
        return *this;
    }

    inline Quaternion& Quaternion::operator -= (const Quaternion& q) noexcept
    {
        xyzw_simd = _mm_sub_ps(xyzw_simd, q.xyzw_simd);
        return *this;
    }

    inline Quaternion& Quaternion::operator *= (const Quaternion& q) noexcept
    {
        Vec3<float> temp (v);
        v = q.v * w + temp * q.w + cross(temp, q.v);
        w = w * q.w - dot(temp, q.v);
        return *this;
    }

    inline Quaternion& Quaternion::operator *= (float f) noexcept
    {
        xyzw_simd = _mm_mul_ps(xyzw_simd, _mm_set1_ps(f));
        return *this;
    }

    inline Quaternion& Quaternion::operator /= (float f) noexcept
    {
        assert(f != static_cast<float>(0u));
        xyzw_simd = _mm_mul_ps(xyzw_simd, _mm_set1_ps(1.0f / f));
        return *this;
    }

    inline float Quaternion::operator [] (size_t i) const noexcept
    {
        assert(i < 4);
        return xyzw[i];
    }

    inline float& Quaternion::operator [] (size_t i) noexcept
    {
        assert(i < 4);
        return xyzw[i];
    }

    ostream& operator << (ostream& print, const Quaternion& q)
    {
        print << q.v << ' ' << q.w;
        return print;
    }

    inline float Quaternion::lengthSquared() const noexcept
    {
        __m128 t = _mm_mul_ps(xyzw_simd, xyzw_simd);
        return t[0] + t[1] + t[2] + t[3];
    }

    inline float Quaternion::length() const noexcept
    {
        __m128 t = _mm_mul_ps(xyzw_simd, xyzw_simd);
        return sqrt(t[0] + t[1] + t[2] + t[3]);
    }

    inline void Quaternion::normalize()
    {
        __m128  t = _mm_mul_ps(xyzw_simd, xyzw_simd);
        float   l = sqrt(t[0] + t[1] + t[2] + t[3]);
        assert(l != static_cast<float>(0u));
        xyzw_simd = _mm_mul_ps(xyzw_simd, _mm_set1_ps(1.0f / l));
    }

    inline void Quaternion::inverse()
    {
        auto ls = lengthSquared();
        *this = conjugate(*this) / ls;
    }

    inline Transform Quaternion::toTransform() const
    {
        float xx = v.x * v.x;
        float yy = v.y * v.y;
        float zz = v.z * v.z;
        float xz = v.x * v.z;
        float xy = v.x * v.y;
        float yz = v.y * v.z;
        float wx = w * v.x;
        float wy = w * v.y;
        float wz = w * v.z;

        return Transform(Matrix4x4<float>(
            1.0f - 2.0f * (yy + zz),    2.0f * (xy + wz),           2.0f * (xz - wy),           0.0f,
            2.0f * (xy - wz),           1.0f - 2.0f * (xx +  zz),   2.0f * (yz + wx),           0.0f,
            2.0f * (xz + wy),           2.0f * (yz - wx),           1.0f - 2.0f * (xx +  yy),   0.0f,
            0.0f,                       0.0f,                       0.0f,                       1.0f
        ));
    }

    inline Quaternion Quaternion::i() noexcept
    {
        return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    }

    inline Quaternion Quaternion::j() noexcept
    {
        return Quaternion(0.0f, 1.0f, 0.0f, 0.0f);
    }

    inline Quaternion Quaternion::k() noexcept
    {
        return Quaternion(0.0f, 0.0f, 1.0f, 0.0f);
    }

    inline Quaternion Quaternion::identity() noexcept
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    inline Quaternion Quaternion::zerro() noexcept
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
    }

    inline float dot(const Quaternion& q1, const Quaternion& q2)
    {
        __m128 t = _mm_mul_ps(q1.xyzw_simd, q2.xyzw_simd);
        return t[0] + t[1] + t[2] + t[3];
    }

    inline Quaternion normalize(const Quaternion& q)
    {
        Quaternion tq (q);
        tq.normalize();
        return tq;
    }

    inline Quaternion inverse(const Quaternion& q)
    {
        Quaternion tq (q);
        tq.inverse();
        return tq;
    }

    inline Quaternion conjugate(const Quaternion& q)
    {
        return Quaternion(q.v * -1.0f, q.w);
    }

    inline Quaternion lerp(float t, const Quaternion& q1, const Quaternion& q2)
    {
        return q1 * (1.0f - t) + q2 * t;
    }

    inline Quaternion slerp(float t, const Quaternion& q1, const Quaternion& q2)
    {
        float cos_theta = dot(q1, q2);

        if (cos_theta > 0.995f) {
            return lerp(t, q1, q2);
        }

        Quaternion  q3 = q2;

        if (cos_theta < static_cast<float>(0u)) {
            q3          *=  -1.0f;
            cos_theta   =   -cos_theta;
        }
        
        float theta = acos(cos_theta);

        return (q1 * sin((1.0f - t) * theta) + q3 * sin(t * theta)) / sin(theta);
    }
}
