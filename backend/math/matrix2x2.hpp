#pragma once

#include <backend/math/vec2.hpp>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    class Matrix2x2 final
    {
    public:
        inline constexpr Matrix2x2()                noexcept;
        inline constexpr Matrix2x2(Type init_value) noexcept;

        inline constexpr Matrix2x2 (
            Type x0, Type y0,
            Type x1, Type y1
        ) noexcept;

        inline constexpr bool operator == (const Matrix2x2& mat) const noexcept;
        inline constexpr bool operator != (const Matrix2x2& mat) const noexcept;

        inline constexpr Matrix2x2    operator + (const Matrix2x2& mat)   const noexcept;
        inline constexpr Matrix2x2    operator - (const Matrix2x2& mat)   const noexcept;
        inline constexpr Matrix2x2    operator * (const Matrix2x2& mat)   const noexcept;
        inline constexpr Vec2<Type>   operator * (const Vec2<Type>& v)    const noexcept;

        inline constexpr Matrix2x2& operator += (const Matrix2x2& mat)    noexcept;
        inline constexpr Matrix2x2& operator -= (const Matrix2x2& mat)    noexcept;
        inline constexpr Matrix2x2& operator *= (const Matrix2x2& mat)    noexcept;
        inline constexpr Matrix2x2& operator *= (Type scal)               noexcept;

        inline Type*        operator [] (size_t i) noexcept;
        inline const Type*  operator [] (size_t i) const noexcept;

        inline Type&            at(size_t i, size_t j);
        inline constexpr Type   at(size_t i, size_t j) const noexcept;

        inline constexpr Type det() const noexcept;

        inline void             inverse() noexcept;
        inline constexpr void   transpose() noexcept;

    private:
        union
        {
            Type _array2x2[2][2];
            Type _array4[4];
        };
    };

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type> transpose(const Matrix2x2<Type>& mat) noexcept;

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type> inverse(const Matrix2x2<Type>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> operator * (const Matrix2x2<T>& mat, T s);

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> operator * (T s, const Matrix2x2<T>& mat);
}

#include <backend/math/matrix2x2.inl>
