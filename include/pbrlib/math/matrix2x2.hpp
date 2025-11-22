#pragma once

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec2;

    template<MathArithmetic T>
    class Matrix3x3;
    
    template<MathArithmetic T>
    class Matrix4x4;
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    class Matrix2x2 final
    {
    public:
        inline constexpr Matrix2x2()                noexcept;
        inline constexpr Matrix2x2(T init_value)    noexcept;

        inline constexpr Matrix2x2 (
            T x0, T y0,
            T x1, T y1
        ) noexcept;

        inline constexpr Matrix2x2(const Matrix3x3<T>& mat) noexcept;
        inline constexpr Matrix2x2(const Matrix4x4<T>& mat) noexcept;

        inline constexpr bool operator == (const Matrix2x2& mat) const noexcept;
        inline constexpr bool operator != (const Matrix2x2& mat) const noexcept;

        inline constexpr Matrix2x2  operator + (const Matrix2x2& mat)   const noexcept;
        inline constexpr Matrix2x2  operator - (const Matrix2x2& mat)   const noexcept;
        inline constexpr Matrix2x2  operator * (const Matrix2x2& mat)   const noexcept;
        inline constexpr Vec2<T>    operator * (const Vec2<T>& v)       const noexcept;

        inline constexpr Matrix2x2& operator += (const Matrix2x2& mat)  noexcept;
        inline constexpr Matrix2x2& operator -= (const Matrix2x2& mat)  noexcept;
        inline constexpr Matrix2x2& operator *= (const Matrix2x2& mat)  noexcept;
        inline constexpr Matrix2x2& operator *= (T scal)                noexcept;

        inline T*       operator [] (size_t i) noexcept;
        inline const T* operator [] (size_t i) const noexcept;

        inline T&           at(size_t i, size_t j);
        inline constexpr T  at(size_t i, size_t j) const noexcept;

        inline constexpr T det() const noexcept;

        inline void             inverse()   noexcept;
        inline constexpr void   transpose() noexcept;

    private:
        union
        {
            T _array2x2[2][2];
            T _array4[4];
        };
    };

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> transpose(const Matrix2x2<T>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> inverse(const Matrix2x2<T>& mat) noexcept;

    template<MathArithmetic T>
    inline Matrix2x2<T> round(const Matrix2x2<T>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> operator * (const Matrix2x2<T>& mat, T s);

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> operator * (T s, const Matrix2x2<T>& mat);
}

namespace pbrlib::math
{
    using mat2      = Matrix2x2<float>;
    using dmat2     = Matrix2x2<double>;
    using imat2     = Matrix2x2<int32_t>;
    using umat2     = Matrix2x2<uint32_t>;
    using i16mat2   = Matrix2x2<int16_t>;
    using u16mat2   = Matrix2x2<uint16_t>;
    using i8mat2    = Matrix2x2<int8_t>;
    using u8mat2    = Matrix2x2<uint8_t>;
}

#include <pbrlib/math/matrix2x2.inl>
