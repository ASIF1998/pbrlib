#pragma once

#include <backend/math/concepts.hpp>
#include <backend/math/vec4.hpp>

namespace pbrlib::math
{
    template<MathArithmetic Type>
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

        inline constexpr bool operator == (const Matrix4x4& mat) const noexcept;
        inline constexpr bool operator != (const Matrix4x4& mat) const noexcept;

        inline constexpr Matrix4x4    operator + (const Matrix4x4& mat)   const noexcept;
        inline constexpr Matrix4x4    operator - (const Matrix4x4& mat)   const noexcept;
        inline constexpr Matrix4x4    operator * (const Matrix4x4& mat)   const noexcept;
        inline constexpr Vec4<Type>   operator * (const Vec4<Type>& v)    const noexcept;

        inline constexpr Matrix4x4& operator += (const Matrix4x4& mat)    noexcept;
        inline constexpr Matrix4x4& operator -= (const Matrix4x4& mat)    noexcept;
        inline constexpr Matrix4x4& operator *= (const Matrix4x4& mat)    noexcept;
        inline constexpr Matrix4x4& operator *= (Type scal)               noexcept;

        inline Type*        operator [] (size_t i) noexcept;
        inline const Type*  operator [] (size_t i) const noexcept;

        inline Type&            at(size_t i, size_t j);
        inline constexpr Type   at(size_t i, size_t j) const noexcept;

        inline constexpr Type det() const noexcept;

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

    template<MathArithmetic Type>
    inline Matrix4x4<Type> transpose(const Matrix4x4<Type>& mat) noexcept;

    template<MathArithmetic Type>
    inline Matrix4x4<Type> inverse(const Matrix4x4<Type>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> operator * (const Matrix4x4<T>& mat, T s);

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> operator * (T s, const Matrix4x4<T>& mat);
}

#include "matrix4x4.inl"
