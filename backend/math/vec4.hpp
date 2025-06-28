#pragma once

#if (defined(__SSE__) || defined(__AVX2__))
#   include <xmmintrin.h>
#endif

#include "vec3.hpp"
#include "vec2.hpp"

namespace pbrlib::math
{
    template<typename Type>
    struct Vec4
    {
    public:
        inline constexpr Vec4(Type xyzw = static_cast<Type>(0))                                             noexcept;
        inline constexpr Vec4(Type x, Type y, Type z = static_cast<Type>(0), Type w = static_cast<Type>(0)) noexcept;
        inline constexpr Vec4(const Vec3<Type>& vec3, Type w = static_cast<Type>(0))                        noexcept;
        inline constexpr Vec4(const Vec2<Type>& v1, const Vec2<Type>& v2)                                   noexcept;
        inline constexpr Vec4(const Vec2<Type>& vec2)                                                       noexcept;

        inline bool operator == (const Vec4& v) const noexcept;
        inline bool operator != (const Vec4& v) const noexcept;

        inline Vec4 operator + (const Vec4& v)  const noexcept;
        inline Vec4 operator - (const Vec4& v)  const noexcept;
        inline Vec4 operator * (Type s)         const noexcept;

        inline Vec4& operator += (const Vec4& v)    noexcept;
        inline Vec4& operator -= (const Vec4& v)    noexcept;
        inline Vec4& operator *= (Type s)           noexcept;

        inline Type&    operator [] (size_t i) noexcept;
        inline Type     operator [] (size_t i) const noexcept;

        inline Type lengthSquared() const noexcept;
        inline Type length()        const noexcept;

        inline void normalize();

    public:
        union
        {
            struct
            {
                Type x;
                Type y;
                Type z;
                Type w;
            };

            struct
            {
                Type r;
                Type g;
                Type b;
                Type a;
            };

            Type xyzw[4];
            Type rgba[4];
        };
    };

#if (defined(__SSE__) || defined(__AVX2__))
    template<>
    struct Vec4<float>
    {
    public:
        inline              Vec4(float xyzw = 0.0f)                                 noexcept;
        inline constexpr    Vec4(__m128 xyzw)                                       noexcept;
        inline constexpr    Vec4(float x, float y, float z = 0.0f, float w = 0.0f)  noexcept;
        inline constexpr    Vec4(const Vec3<float>& vec3, float w = 0.0f)                  noexcept; 
        inline constexpr    Vec4(const Vec2<float>& v1, const Vec2<float>& v2)                    noexcept;
        inline constexpr    Vec4(const Vec2<float>& vec2)                                  noexcept;

        inline bool operator == (const Vec4& v) const noexcept;
        inline bool operator != (const Vec4& v) const noexcept;

        inline Vec4 operator + (const Vec4& v)  const noexcept;
        inline Vec4 operator - (const Vec4& v)  const noexcept;
        inline Vec4 operator * (float s)        const noexcept;

        inline Vec4& operator += (const Vec4& v)    noexcept;
        inline Vec4& operator -= (const Vec4& v)    noexcept;
        inline Vec4& operator *= (float s)          noexcept;

        inline float&   operator [] (size_t i) noexcept;
        inline float    operator [] (size_t i) const noexcept;

        inline float lengthSquared() const noexcept;
        inline float length() const;

        inline void normalize();

    public:
        union
        {
            struct 
            {
                float x;
                float y;
                float z;
                float w;
            };
            
            struct
            {
                float r;
                float g;
                float b;
                float a;
            };

            float   xyzw[4];
            float   rgba[4];
            __m128  xyzw_simd;
        };
    };
#endif

    template<typename Type>
    inline Type dot(const Vec4<Type> v1, const Vec4<Type>& v2) noexcept;

#if (defined(__SSE__) || defined(__AVX2__))
    template<>
    inline float dot<float>(const Vec4<float> v1, const Vec4<float>& v2);
#endif

    template<typename Type>
    inline Vec4<Type> normalize(const Vec4<Type>& v);
}

#include "vec4.inl"
