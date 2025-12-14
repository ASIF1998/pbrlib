#pragma once

#include <pbrlib/math/concepts.hpp>

#include <format>

namespace pbrlib::math
{
    template<MathArithmetic T>
    struct Vec3;

    template<MathArithmetic T>
    class Matrix4x4;
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    class Matrix3x3 final
    {
    public:
        inline constexpr Matrix3x3()                noexcept;
        inline constexpr Matrix3x3(T init_value)    noexcept;

        inline constexpr Matrix3x3 (
            T x0, T y0, T z0,
            T x1, T y1, T z1,
            T x2, T y2, T z2 
        ) noexcept;

        inline constexpr Matrix3x3(const Matrix4x4<T>& mat) noexcept;

        inline constexpr bool operator == (const Matrix3x3& mat) const noexcept;
        inline constexpr bool operator != (const Matrix3x3& mat) const noexcept;

        inline constexpr Matrix3x3  operator + (const Matrix3x3& mat)   const noexcept;
        inline constexpr Matrix3x3  operator - (const Matrix3x3& mat)   const noexcept;
        inline constexpr Matrix3x3  operator * (const Matrix3x3& mat)   const noexcept;
        inline constexpr Vec3<T>    operator * (const Vec3<T>& v)       const noexcept;

        inline constexpr Matrix3x3& operator += (const Matrix3x3& mat)  noexcept;
        inline constexpr Matrix3x3& operator -= (const Matrix3x3& mat)  noexcept;
        inline constexpr Matrix3x3& operator *= (const Matrix3x3& mat)  noexcept;
        inline constexpr Matrix3x3& operator *= (T scal)                noexcept;

        inline T*        operator [] (size_t i) noexcept;
        inline const T*  operator [] (size_t i) const noexcept;

        inline T&            at(size_t i, size_t j);
        inline constexpr T   at(size_t i, size_t j) const;

        inline constexpr T det() const noexcept;

        inline void             inverse()   noexcept;
        inline constexpr void   transpose() noexcept;

    private:
        union
        {
            T _array3x3[3][3];
            T _array9[9];
        };
    };

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> transpose(const Matrix3x3<T>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> inverse(const Matrix3x3<T>& mat) noexcept;

    template<MathArithmetic T>
    inline Matrix3x3<T> round(const Matrix3x3<T>& mat) noexcept;

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> operator * (const Matrix3x3<T>& mat, T s);

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> operator * (T s, const Matrix3x3<T>& mat);
}

namespace std
{
    template<pbrlib::math::MathArithmetic T>
    struct formatter<pbrlib::math::Matrix3x3<T>>
    {
        constexpr auto  parse(format_parse_context& ctx)                                    const;
        auto            format(const pbrlib::math::Matrix3x3<T>& mat, format_context& ctx)  const;
    };

    template<pbrlib::math::MathArithmetic T>
    struct hash<pbrlib::math::Matrix3x3<T>> final
    {
        inline constexpr size_t operator () (const pbrlib::math::Matrix3x3<T>& mat) const noexcept;
    };
}

namespace pbrlib::math
{
    using mat3      = Matrix3x3<float>;
    using dmat3     = Matrix3x3<double>;
    using imat3     = Matrix3x3<int32_t>;
    using umat3     = Matrix3x3<uint32_t>;
    using i16mat3   = Matrix3x3<int16_t>;
    using u16mat3   = Matrix3x3<uint16_t>;
    using i8mat3    = Matrix3x3<int8_t>;
    using u8mat3    = Matrix3x3<uint8_t>;
}

#include <pbrlib/math/matrix3x3.inl>