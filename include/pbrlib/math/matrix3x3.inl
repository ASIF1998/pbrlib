#include <pbrlib/exceptions.hpp>

#include <pbrlib/math/matrix4x4.hpp>

#include <memory>
#include <algorithm>
#include <format>

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>::Matrix3x3() noexcept :
        _array9 
        {
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)
        }
    {}

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>::Matrix3x3(T init_value) noexcept :
        _array9 
        {
            init_value, static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), init_value, static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), init_value
        }
    {}

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>::Matrix3x3 (
        T x0, T y0, T z0,
        T x1, T y1, T z1,
        T x2, T y2, T z2
    ) noexcept :
        _array9 
        {
            x0, y0, z0,
            x1, y1, z1,
            x2, y2, z2
        }
    {}

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& mat) noexcept 
    {
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
                _array3x3[i][j] = mat.at(i, j);
        }
    }

    template<MathArithmetic T>
    inline constexpr bool Matrix3x3<T>::operator == (const Matrix3x3<T>& mat) const noexcept
    {
        bool res = true;

        for (size_t i{0}; i < 9; i++) 
            res &= _array9[i] == mat._array9[i];

        return res;
    }

    template<MathArithmetic T>
    inline constexpr bool Matrix3x3<T>::operator != (const Matrix3x3<T>& mat) const noexcept
    {
        bool res = true;

        for (size_t i{0}; i < 9; i++)
            res |= _array9[i] != mat._array9[i];

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> Matrix3x3<T>::operator + (const Matrix3x3<T>& mat) const noexcept
    {
        Matrix3x3<T> res;

        for (size_t i{0}; i < 9; i++)
            res._array9[i] = _array9[i] + mat._array9[i];

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> Matrix3x3<T>::operator - (const Matrix3x3<T>& mat) const noexcept
    {
        Matrix3x3<T> res;

        for (size_t i{0}; i < 9; i++) 
            res._array9[i] = _array9[i] - mat._array9[i];

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> Matrix3x3<T>::operator * (const Matrix3x3<T>& mat) const noexcept
    {
        Matrix3x3<T> res (static_cast<T>(0));

        for (size_t i = 0; i < 3; i++) 
        {
            for (size_t k = 0; k < 3; k++) 
            {
                auto v = mat._array3x3[i][k];
                for (size_t j = 0; j < 3; j++) 
                    res._array3x3[i][j] += v * _array3x3[k][j];
            }
        }

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> operator * (const Matrix3x3<T>& mat, T s)
    {
        Matrix3x3<T> res;
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
                res.at(i, j) = mat.at(i, j) * s;
        }

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> operator * (T s, const Matrix3x3<T>& mat)
    {
        Matrix3x3<T> res;
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
                res.at(i, j) = mat.at(i, j) * s;
        }

        return res;
    }   

    template<MathArithmetic T>
    inline constexpr Vec3<T> Matrix3x3<T>::operator * (const Vec3<T>& v) const noexcept
    {
        Vec3<T> res;

        for (size_t i = 0; i < 3; i++) 
        {
            res.x += _array3x3[i][0] * v[i];
            res.y += _array3x3[i][1] * v[i];
            res.z += _array3x3[i][2] * v[i];
        }

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>& Matrix3x3<T>::operator += (const Matrix3x3<T>& mat) noexcept
    {
        for (size_t i = 0; i < 9; i++)
            _array9[i] += mat._array9[i];

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>& Matrix3x3<T>::operator -= (const Matrix3x3<T>& mat) noexcept
    {
        for (size_t i = 0; i < 9; i++)
            _array9[i] -= mat._array9[i];

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>& Matrix3x3<T>::operator *= (const Matrix3x3<T>& mat) noexcept
    {
        *this = *this * mat;
        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T>& Matrix3x3<T>::operator *= (T scal) noexcept
    {
        for (size_t i = 0; i < 9; i++) 
            _array9[i] *= scal;

        return *this;
    }

    template<MathArithmetic T>
    inline T* Matrix3x3<T>::operator [] (size_t i) noexcept
    {
        return _array3x3[i];
    }

    template<MathArithmetic T>
    inline const T* Matrix3x3<T>::operator [] (size_t i) const noexcept
    {
        return _array3x3[i];
    }

    template<MathArithmetic T>
    inline T& Matrix3x3<T>::at(size_t i, size_t j)
    {
        if (i > 2 || j > 2) [[unlikely]] 
            throw exception::InvalidArgument(std::format("[math::mat3] i = {}, j = {}", i, j)); 

        return _array3x3[i][j];
    }

    template<MathArithmetic T>
    inline constexpr T Matrix3x3<T>::at(size_t i, size_t j) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct conditional access based on i and j
        return (i == 0 && j == 0) ? _array9[0] : 
               (i == 0 && j == 1) ? _array9[1] : 
               (i == 0 && j == 2) ? _array9[2] : 
               (i == 1 && j == 0) ? _array9[3] : 
               (i == 1 && j == 1) ? _array9[4] : 
               (i == 1 && j == 2) ? _array9[5] : 
               (i == 2 && j == 0) ? _array9[6] : 
               (i == 2 && j == 1) ? _array9[7] : _array9[8];
    }

    template<MathArithmetic T>
    inline constexpr T Matrix3x3<T>::det() const noexcept
    {
        return 
                (at(0, 0) * at(1, 1) * at(2, 2)) 
            +   (at(2, 0) * at(0, 1) * at(1, 2)) 
            +   (at(1, 0) * at(2, 1) * at(0, 2)) 
            -   (at(2, 0) * at(1, 1) * at(0, 2)) 
            -   (at(0, 0) * at(2, 1) * at(1, 2)) 
            -   (at(1, 0) * at(0, 1) * at(2, 2));
    }

    template<MathArithmetic T>
    inline constexpr void Matrix3x3<T>::transpose() noexcept
    {
        std::swap(at(0, 1), at(1, 0));
        std::swap(at(0, 2), at(2, 0));
        std::swap(at(1, 2), at(2, 1));
    }

    template<MathArithmetic T>
    inline void Matrix3x3<T>::inverse() noexcept
    {
        auto d = det();

        if (d != T(0)) [[likely]] 
        {
            auto tmat (*this);

            tmat.transpose();

            _array3x3[0][0] =   (tmat._array3x3[1][1] * tmat._array3x3[2][2] - tmat._array3x3[1][2] * tmat._array3x3[2][1]);
            _array3x3[0][1] = - (tmat._array3x3[1][0] * tmat._array3x3[2][2] - tmat._array3x3[1][2] * tmat._array3x3[2][0]);
            _array3x3[0][2] =   (tmat._array3x3[1][0] * tmat._array3x3[2][1] - tmat._array3x3[1][1] * tmat._array3x3[2][0]);

            _array3x3[1][0] = - (tmat._array3x3[0][1] * tmat._array3x3[2][2] - tmat._array3x3[0][2] * tmat._array3x3[2][1]);
            _array3x3[1][1] =   (tmat._array3x3[0][0] * tmat._array3x3[2][2] - tmat._array3x3[0][2] * tmat._array3x3[2][0]);
            _array3x3[1][2] = - (tmat._array3x3[0][0] * tmat._array3x3[2][1] - tmat._array3x3[0][1] * tmat._array3x3[2][0]);

            _array3x3[2][0] =   (tmat._array3x3[0][1] * tmat._array3x3[1][2] - tmat._array3x3[0][2] * tmat._array3x3[1][1]);
            _array3x3[2][1] = - (tmat._array3x3[0][0] * tmat._array3x3[1][2] - tmat._array3x3[0][2] * tmat._array3x3[1][0]);
            _array3x3[2][2] =   (tmat._array3x3[0][0] * tmat._array3x3[1][1] - tmat._array3x3[0][1] * tmat._array3x3[1][0]);

            for (size_t i = 0; i < 9; i++)
                _array9[i] /= d;
        }
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> transpose(const Matrix3x3<T>& mat) noexcept
    {
        return 
        {
            mat.at(0, 0), mat.at(1, 0), mat.at(2, 0),
            mat.at(0, 1), mat.at(1, 1), mat.at(2, 1),
            mat.at(0, 2), mat.at(1, 2), mat.at(2, 2)
        };
    }

    template<MathArithmetic T>
    inline constexpr Matrix3x3<T> inverse(const Matrix3x3<T>& mat) noexcept
    {
        Matrix3x3<T> res (mat);
        res.inverse();
        return res;
    }

    template<MathArithmetic T>
    inline Matrix3x3<T> round(const Matrix3x3<T>& mat) noexcept
    {
        const Matrix3x3<T> res (
            std::round(mat.at(0, 0)), std::round(mat.at(0, 1)), std::round(mat.at(0, 2)),
            std::round(mat.at(1, 0)), std::round(mat.at(1, 1)), std::round(mat.at(1, 2)),
            std::round(mat.at(2, 0)), std::round(mat.at(2, 1)), std::round(mat.at(2, 2))
        );

        return res;
    }
}

namespace std
{
    template<pbrlib::math::MathArithmetic T>
    constexpr auto formatter<pbrlib::math::Matrix3x3<T>>::parse(format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    template<pbrlib::math::MathArithmetic T>
    auto formatter<pbrlib::math::Matrix3x3<T>>::format (
        const pbrlib::math::Matrix3x3<T>&   mat, 
        format_context&                     ctx
    ) const
    {
        return format_to (
            ctx.out(), 
            "\nmat3 [\n  [{}, {}, {}]\n  [{}, {}, {}]\n  [{}, {}, {}]\n]", 
                mat[0][0], mat[0][1], mat[0][2], 
                mat[1][0], mat[1][1], mat[1][2], 
                mat[2][0], mat[2][1], mat[2][2]
        );
    }
}