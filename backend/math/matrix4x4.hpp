#pragma once

#if defined(__AVX2__)
#   include <immintrin.h>
#endif

#include "vec4.hpp"

namespace pbrlib::math
{
    template<typename Type>
    class Matrix4x4
    {
    public:
        inline constexpr Matrix4x4() noexcept;

        inline constexpr Matrix4x4(Type init_value) noexcept;

        inline constexpr Matrix4x4 (
            Type x0, Type y0, Type z0, Type w0,
            Type x1, Type y1, Type z1, Type w1,
            Type x2, Type y2, Type z2, Type w2,
            Type x3, Type y3, Type z3, Type w3
        ) noexcept;

        inline bool operator == (const Matrix4x4& mat) const noexcept;
        inline bool operator != (const Matrix4x4& mat) const noexcept;

        inline Matrix4x4    operator + (const Matrix4x4& mat)   const noexcept;
        inline Matrix4x4    operator - (const Matrix4x4& mat)   const noexcept;
        inline Matrix4x4    operator * (const Matrix4x4& mat)   const noexcept;
        inline Matrix4x4    operator * (Type scal)              const noexcept;
        inline Vec4<Type>   operator * (const Vec4<Type>& v)    const noexcept;

        inline Matrix4x4& operator += (const Matrix4x4& mat)    noexcept;
        inline Matrix4x4& operator -= (const Matrix4x4& mat)    noexcept;
        inline Matrix4x4& operator *= (const Matrix4x4& mat)    noexcept;
        inline Matrix4x4& operator *= (Type scal)               noexcept;

        inline Type*        operator [] (size_t i) noexcept;
        inline const Type*  operator [] (size_t i) const noexcept;

        inline Type&    at(size_t i, size_t j);
        inline Type     at(size_t i, size_t j) const;

        inline Type det() const noexcept;

        inline void transpose() noexcept;
        inline void inverse()   noexcept;

    private:
        union
        {
            Type        _array4x4[4][4];
            Type        _array16[16];
            Vec4<Type>  _vec_array[4];
        };
    };

#if defined(__AVX2__)
    template<>
    class Matrix4x4<float>
    {
    public:
        inline constexpr Matrix4x4();
        
        inline constexpr Matrix4x4(float init_value);

        inline constexpr Matrix4x4(
            float x0, float y0, float z0, float w0,
            float x1, float y1, float z1, float w1,
            float x2, float y2, float z2, float w2,
            float x3, float y3, float z3, float w3
        );

        inline bool operator == (const Matrix4x4& mat) const;
        inline bool operator != (const Matrix4x4& mat) const;

        inline Matrix4x4    operator + (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator - (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator * (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator * (float scal)             const;
        inline Vec4<float>  operator * (const Vec4<float>& v)   const;

        inline Matrix4x4& operator += (const Matrix4x4& mat);
        inline Matrix4x4& operator -= (const Matrix4x4& mat);
        inline Matrix4x4& operator *= (const Matrix4x4& mat);
        inline Matrix4x4& operator *= (float scal);

        inline float*       operator [] (size_t i) noexcept;
        inline const float* operator [] (size_t i) const noexcept;

        inline float&   at (size_t i, size_t j);
        inline float    at (size_t i, size_t j) const;

        inline float det() const;

        inline void transpose();
        inline void inverse();

    private:
        inline Matrix4x4(const __m256& s1, const __m256& s2);

        union
        {
            float   _array4x4[4][4];
            float   _array16[16];
            __m128  _m128_simd[4];
            __m256  _m256_simd[2];
        };
    };
#endif

    template<typename Type>
    inline Matrix4x4<Type> transpose(const Matrix4x4<Type>& mat) noexcept;

    template<typename Type>
    inline Matrix4x4<Type> inverse(const Matrix4x4<Type>& mat) noexcept;
}

#include "matrix4x4.inl"
