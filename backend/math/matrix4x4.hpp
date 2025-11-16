#pragma once

#include <backend/math/concepts.hpp>
#include <backend/math/vec4.hpp>

namespace pbrlib::math
{
    template<MathArithmetic T>
    class Matrix4x4
    {
    public:
        inline constexpr Matrix4x4() noexcept;

        inline constexpr Matrix4x4(T init_value) noexcept;

        inline constexpr Matrix4x4 (
            T x0, T y0, T z0, T w0,
            T x1, T y1, T z1, T w1,
            T x2, T y2, T z2, T w2,
            T x3, T y3, T z3, T w3
        ) noexcept;

        inline constexpr bool operator == (const Matrix4x4& mat) const noexcept;
        inline constexpr bool operator != (const Matrix4x4& mat) const noexcept;

        inline constexpr Matrix4x4  operator + (const Matrix4x4& mat)   const noexcept;
        inline constexpr Matrix4x4  operator - (const Matrix4x4& mat)   const noexcept;
        inline constexpr Matrix4x4  operator * (const Matrix4x4& mat)   const noexcept;
        inline constexpr Vec4<T>    operator * (const Vec4<T>& v)       const noexcept;

        inline constexpr Matrix4x4& operator += (const Matrix4x4& mat)  noexcept;
        inline constexpr Matrix4x4& operator -= (const Matrix4x4& mat)  noexcept;
        inline constexpr Matrix4x4& operator *= (const Matrix4x4& mat)  noexcept;
        inline constexpr Matrix4x4& operator *= (T scal)                noexcept;

        inline T*        operator [] (size_t i) noexcept;
        inline const T*  operator [] (size_t i) const noexcept;

        inline T&            at(size_t i, size_t j);
        inline constexpr T   at(size_t i, size_t j) const noexcept;

        inline constexpr T det() const noexcept;

        inline void transpose() noexcept;
        inline void inverse()   noexcept;

    private:
        union
        {
            T        _array4x4[4][4];
            T        _array16[16];
            Vec4<T>  _vec_array[4];
        };
    };

    template<MathArithmetic T>
    inline Matrix4x4<T> transpose(const Matrix4x4<T>& mat) noexcept;

    template<MathArithmetic T>
    inline Matrix4x4<T> inverse(const Matrix4x4<T>& mat) noexcept;

    template<MathArithmetic T>
    inline Matrix4x4<T> round(const Matrix4x4<T>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> operator * (const Matrix4x4<T>& mat, T s);

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> operator * (T s, const Matrix4x4<T>& mat);
}

#include <backend/math/matrix4x4.inl>
