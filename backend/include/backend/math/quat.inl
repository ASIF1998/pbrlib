namespace pbrlib::math
{
    inline constexpr Quaternion::Quaternion() :
        v(0),
        w(1)
    {}

    inline constexpr Quaternion::Quaternion(float x, float y, float z, float w) :   
        v(x, y, z),
        w(w)
    {}

    inline constexpr Quaternion::Quaternion(const vec3& v, float w) :
        v(v),
        w(w)
    {}

#if (defined(__SSE__) || defined(__AVX2__))
    inline constexpr Quaternion::Quaternion(const __m128& m) noexcept :
        xyzw_simd(m)
    {}
#endif

    inline bool Quaternion::operator == (const Quaternion& q) const noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw_simd, q.xyzw_simd)) == 15;
#else 
        return v == q.v && w == q.w;
#endif
    }

    inline bool Quaternion::operator != (const Quaternion& q) const noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw_simd, q.xyzw_simd)) != 15;
#else   
        return v != q.v || w != q.w;
#endif
    }
    
    inline Quaternion Quaternion::operator + (const Quaternion& q) const noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        return Quaternion(_mm_add_ps(xyzw_simd, q.xyzw_simd));
#else
        return Quaternion(v + q.v, w + q.w);
#endif
    }

    inline Quaternion Quaternion::operator - (const Quaternion& q) const noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        return Quaternion(_mm_sub_ps(xyzw_simd, q.xyzw_simd));
#else   
        return Quaternion(v - q.v, w - q.w);
#endif
    }

    inline Quaternion Quaternion::operator * (const Quaternion& q) const noexcept
    {
        return Quaternion(q.v * w + v * q.w + cross(v, q.v), w * q.w - dot(v, q.v));
    }

    inline Quaternion Quaternion::operator * (float f) const noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        return Quaternion(_mm_mul_ps(xyzw_simd, _mm_set1_ps(f)));
#else
        return Quaternion(v * f, w * f);
#endif
    }

    inline Quaternion Quaternion::operator / (float f) const noexcept
    {
        assert(f != static_cast<float>(0u));
#if (defined(__SSE__) || defined(__AVX2__))
        return Quaternion(_mm_mul_ps(xyzw_simd, _mm_set1_ps(1.0f / f)));
#else 
        float inv_f = 1.0f / f;
        return Quaternion(v * inv_f, w * inv_f);
#endif
    }

    inline Quaternion& Quaternion::operator += (const Quaternion& q) noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        xyzw_simd = _mm_add_ps(xyzw_simd, q.xyzw_simd);
#else
        v += q.v;
        w += q.w;
#endif
        return *this;
    }

    inline Quaternion& Quaternion::operator -= (const Quaternion& q) noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        xyzw_simd = _mm_sub_ps(xyzw_simd, q.xyzw_simd);
#else
        v -= q.v;
        w -= q.w;
#endif
        return *this;
    }

    inline Quaternion& Quaternion::operator *= (const Quaternion& q) noexcept
    {
        vec3 temp (v);
        v = q.v * w + temp * q.w + cross(temp, q.v);
        w = w * q.w - dot(temp, q.v);
        return *this;
    }

    inline Quaternion& Quaternion::operator *= (float f) noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        xyzw_simd = _mm_mul_ps(xyzw_simd, _mm_set1_ps(f));
#else
        v *= f;
        w *= f;
#endif
        return *this;
    }

    inline Quaternion& Quaternion::operator /= (float f) noexcept
    {
        assert(f != static_cast<float>(0u));
#if (defined(__SSE__) || defined(__AVX2__))
        xyzw_simd = _mm_mul_ps(xyzw_simd, _mm_set1_ps(1.0f / f));
#else 
        float inv_f = 1.0f / f;

        v *= inv_f;
        w *= inv_f;
#endif    
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

    inline float Quaternion::lengthSquared() const noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        float t[4];
        _mm_store_ps(t, _mm_mul_ps(xyzw_simd, xyzw_simd));
        return t[0] + t[1] + t[2] + t[3];
#else
        return xyzw[0] * xyzw[0] + xyzw[1] * xyzw[1] + xyzw[2] * xyzw[2] + xyzw[3] * xyzw[3];
#endif
    }

    inline float Quaternion::length() const noexcept
    {
#if (defined(__SSE__) || defined(__AVX2__))
        float t[4];
        _mm_store_ps(t, _mm_mul_ps(xyzw_simd, xyzw_simd));
        return sqrt(t[0] + t[1] + t[2] + t[3]);
#else 
        return sqrt(xyzw[0] * xyzw[0] + xyzw[1] * xyzw[1] + xyzw[2] * xyzw[2] + xyzw[3] * xyzw[3]);
#endif
    }

    inline void Quaternion::normalize()
    {
#if (defined(__SSE__) || defined(__AVX2__))
        float t[4];
        _mm_store_ps(t, _mm_mul_ps(xyzw_simd, xyzw_simd));

        float   l = sqrt(t[0] + t[1] + t[2] + t[3]);
        assert(l != static_cast<float>(0u));
        xyzw_simd = _mm_mul_ps(xyzw_simd, _mm_set1_ps(1.0f / l));
#else
        float l = length();

        assert(l != static_cast<float>(0u));

        l = 1.0f / l;

        v *= l;
        w *= l;
#endif
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

        return Transform(mat4(
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
#if (defined(__SSE__) || defined(__AVX2__))
        float t[4];
        _mm_store_ps(t, _mm_mul_ps(q1.xyzw_simd, q2.xyzw_simd));
        return t[0] + t[1] + t[2] + t[3];
#else
        return q1.xyzw[0] * q2.xyzw[0] + q1.xyzw[1] * q2.xyzw[1] + q1.xyzw[2] * q2.xyzw[2] + q1.xyzw[3] * q2.xyzw[3];
#endif
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
