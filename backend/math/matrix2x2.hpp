#pragma once

#if (defined(__SSE__) || defined(__AVX2__))
#   include <xmmintrin.h>
#endif

namespace pbrlib::math
{
    template<typename Type>
    struct Vec2;

    template<typename Type>
    class Matrix2x2 final
    {
    public:
        inline constexpr Matrix2x2()                noexcept;
        inline constexpr Matrix2x2(Type init_value) noexcept;

        inline constexpr Matrix2x2(
            Type x0, Type y0,
            Type x1, Type y1
        ) noexcept;

        inline bool operator == (const Matrix2x2& mat) const noexcept;
        inline bool operator != (const Matrix2x2& mat) const noexcept;

        inline Matrix2x2    operator + (const Matrix2x2& mat)   const noexcept;
        inline Matrix2x2    operator - (const Matrix2x2& mat)   const noexcept;
        inline Matrix2x2    operator * (const Matrix2x2& mat)   const noexcept;
        inline Matrix2x2    operator * (Type scal)              const noexcept;
        inline Vec2<Type>   operator * (const Vec2<Type>& v)    const noexcept;

        inline Matrix2x2& operator += (const Matrix2x2& mat)    noexcept;
        inline Matrix2x2& operator -= (const Matrix2x2& mat)    noexcept;
        inline Matrix2x2& operator *= (const Matrix2x2& mat)    noexcept;
        inline Matrix2x2& operator *= (Type scal)               noexcept;

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
            Type _array2x2[2][2];
            Type _array4[4];
        };
    };

#if (defined(__SSE__) || defined(__AVX2__))
    template<>
    class Matrix2x2<float>
    {
    public:
        inline constexpr Matrix2x2();

        inline constexpr Matrix2x2(float init_value)        noexcept;
        inline constexpr Matrix2x2(const __m128& init_vec)  noexcept;

        inline constexpr Matrix2x2(
            float x0, float y0,
            float x1, float y1
        )  noexcept;

        inline bool operator == (const Matrix2x2& mat) const;
        inline bool operator != (const Matrix2x2& mat) const;

        inline Matrix2x2    operator + (const Matrix2x2& mat)   const;
        inline Matrix2x2    operator - (const Matrix2x2& mat)   const;
        inline Matrix2x2    operator * (const Matrix2x2& mat)   const;
        inline Matrix2x2    operator * (float scal)             const;
        inline vec2         operator * (const vec2& v)          const;

        inline Matrix2x2& operator += (const Matrix2x2& mat);
        inline Matrix2x2& operator -= (const Matrix2x2& mat);
        inline Matrix2x2& operator *= (const Matrix2x2& mat);
        inline Matrix2x2& operator *= (float scal);

        inline float*       operator [] (size_t i);
        inline const float* operator [] (size_t i) const;

        inline float&   at(size_t i, size_t j);
        inline float    at(size_t i, size_t j) const;

        inline float det() const;

        inline void transpose();

        inline void inverse();

    private:
        union
        {
            float   _array2x2[2][2];
            float   _array4[4];
            __m128  _m128_simd;
        };
    };
#endif

    template<typename Type>
    inline Matrix2x2<Type> transpose(const Matrix2x2<Type>& mat) noexcept;

    template<typename Type>
    Matrix2x2<Type> inverse(const Matrix2x2<Type>& mat) noexcept;
}

#include "matrix2x2.inl"
