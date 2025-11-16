#pragma once

#include <backend/math/concepts.hpp>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    struct Vec3;

    template<MathArithmetic Type>
    class Matrix3x3 final
    {
    public:
        inline constexpr Matrix3x3()                noexcept;
        inline constexpr Matrix3x3(Type init_value) noexcept;

        inline constexpr Matrix3x3 (
            Type x0, Type y0, Type z0,
            Type x1, Type y1, Type z1,
            Type x2, Type y2, Type z2 
        ) noexcept;

        inline constexpr bool operator == (const Matrix3x3& mat) const noexcept;
        inline constexpr bool operator != (const Matrix3x3& mat) const noexcept;

        inline constexpr Matrix3x3  operator + (const Matrix3x3& mat)   const noexcept;
        inline constexpr Matrix3x3  operator - (const Matrix3x3& mat)   const noexcept;
        inline constexpr Matrix3x3  operator * (const Matrix3x3& mat)   const noexcept;
        inline constexpr Vec3<Type> operator * (const Vec3<Type>& v)    const noexcept;

        inline constexpr Matrix3x3& operator += (const Matrix3x3& mat)  noexcept;
        inline constexpr Matrix3x3& operator -= (const Matrix3x3& mat)  noexcept;
        inline constexpr Matrix3x3& operator *= (const Matrix3x3& mat)  noexcept;
        inline constexpr Matrix3x3& operator *= (Type scal)             noexcept;

        inline Type*        operator [] (size_t i) noexcept;
        inline const Type*  operator [] (size_t i) const noexcept;

        inline Type&            at(size_t i, size_t j);
        inline constexpr Type   at(size_t i, size_t j) const noexcept;

        inline constexpr Type det() const noexcept;

        inline void             inverse()   noexcept;
        inline constexpr void   transpose() noexcept;

    private:
        union
        {
            Type _array3x3[3][3];
            Type _array9[9];
        };
    };

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type> transpose(const Matrix3x3<Type>& mat) noexcept;

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type> inverse(const Matrix3x3<Type>& mat) noexcept;

    template<MathArithmetic Type>
    inline Matrix3x3<Type> round(const Matrix3x3<Type>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> operator * (const Matrix3x3<T>& mat, T s);

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> operator * (T s, const Matrix3x3<T>& mat);
}


#include <backend/math/matrix3x3.inl>