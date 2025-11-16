#include <memory>

#include <algorithm>

#include <format>
#include <pbrlib/exceptions.hpp>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type>::Matrix3x3() noexcept :
        _array9 
        {
            static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
        }
    {}

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type>::Matrix3x3(Type init_value) noexcept :
        _array9 
        {
            init_value, static_cast<Type>(0), static_cast<Type>(0),
            static_cast<Type>(0), init_value, static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(0), init_value
        }
    {}

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type>::Matrix3x3 (
        Type x0, Type y0, Type z0,
        Type x1, Type y1, Type z1,
        Type x2, Type y2, Type z2
    ) noexcept :
        _array9 
        {
            x0, y0, z0,
            x1, y1, z1,
            x2, y2, z2
        }
    {}

    template<MathArithmetic Type>
    inline constexpr bool Matrix3x3<Type>::operator == (const Matrix3x3<Type>& mat) const noexcept
    {
        bool res = true;

        for (size_t i{0}; i < 9; i++) 
            res &= _array9[i] == mat._array9[i];

        return res;
    }

    template<MathArithmetic Type>
    inline constexpr bool Matrix3x3<Type>::operator != (const Matrix3x3<Type>& mat) const noexcept
    {
        bool res = true;

        for (size_t i{0}; i < 9; i++)
            res |= _array9[i] != mat._array9[i];

        return res;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type> Matrix3x3<Type>::operator + (const Matrix3x3<Type>& mat) const noexcept
    {
        Matrix3x3<Type> res;

        for (size_t i{0}; i < 9; i++)
            res._array9[i] = _array9[i] + mat._array9[i];

        return res;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type> Matrix3x3<Type>::operator - (const Matrix3x3<Type>& mat) const noexcept
    {
        Matrix3x3<Type> res;

        for (size_t i{0}; i < 9; i++) 
            res._array9[i] = _array9[i] - mat._array9[i];

        return res;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type> Matrix3x3<Type>::operator * (const Matrix3x3<Type>& mat) const noexcept
    {
        Matrix3x3<Type> res (static_cast<Type>(0));

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

    template<MathArithmetic Type>
    inline constexpr Vec3<Type> Matrix3x3<Type>::operator * (const Vec3<Type>& v) const noexcept
    {
        Vec3<Type> res;

        for (size_t i = 0; i < 3; i++) 
        {
            res.x += _array3x3[i][0] * v[i];
            res.y += _array3x3[i][1] * v[i];
            res.z += _array3x3[i][2] * v[i];
        }

        return res;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type>& Matrix3x3<Type>::operator += (const Matrix3x3<Type>& mat) noexcept
    {
        for (size_t i = 0; i < 9; i++)
            _array9[i] += mat._array9[i];

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type>& Matrix3x3<Type>::operator -= (const Matrix3x3<Type>& mat) noexcept
    {
        for (size_t i = 0; i < 9; i++)
            _array9[i] -= mat._array9[i];

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type>& Matrix3x3<Type>::operator *= (const Matrix3x3<Type>& mat) noexcept
    {
        *this = *this * mat;
        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type>& Matrix3x3<Type>::operator *= (Type scal) noexcept
    {
        for (size_t i = 0; i < 9; i++) 
            _array9[i] *= scal;

        return *this;
    }

    template<MathArithmetic Type>
    inline Type* Matrix3x3<Type>::operator [] (size_t i) noexcept
    {
        return _array3x3[i];
    }

    template<MathArithmetic Type>
    inline const Type* Matrix3x3<Type>::operator [] (size_t i) const noexcept
    {
        return _array3x3[i];
    }

    template<MathArithmetic Type>
    inline Type& Matrix3x3<Type>::at(size_t i, size_t j)
    {
        if (i > 2 || j > 2) [[unlikely]] 
            throw exception::InvalidArgument(std::format("[math::mat3] i = {}, j = {}", i, j)); 

        return _array3x3[i][j];
    }

    template<MathArithmetic Type>
    inline constexpr Type Matrix3x3<Type>::at(size_t i, size_t j) const noexcept
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

    template<MathArithmetic Type>
    inline constexpr Type Matrix3x3<Type>::det() const noexcept
    {
        return 
                (at(0, 0) * at(1, 1) * at(2, 2)) 
            +   (at(2, 0) * at(0, 1) * at(1, 2)) 
            +   (at(1, 0) * at(2, 1) * at(0, 2)) 
            -   (at(2, 0) * at(1, 1) * at(0, 2)) 
            -   (at(0, 0) * at(2, 1) * at(1, 2)) 
            -   (at(1, 0) * at(0, 1) * at(2, 2));
    }

    template<MathArithmetic Type>
    inline constexpr void Matrix3x3<Type>::transpose() noexcept
    {
        std::swap(at(0, 1), at(1, 0));
        std::swap(at(0, 2), at(2, 0));
        std::swap(at(1, 2), at(2, 1));
    }

    template<MathArithmetic Type>
    inline void Matrix3x3<Type>::inverse() noexcept
    {
        auto d = det();

        if (d != Type(0)) [[likely]] 
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

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type> transpose(const Matrix3x3<Type>& mat) noexcept
    {
        return 
        {
            mat.at(0, 0), mat.at(1, 0), mat.at(2, 0),
            mat.at(0, 1), mat.at(1, 1), mat.at(2, 1),
            mat.at(0, 2), mat.at(1, 2), mat.at(2, 2)
        };
    }

    template<MathArithmetic Type>
    inline constexpr Matrix3x3<Type> inverse(const Matrix3x3<Type>& mat) noexcept
    {
        Matrix3x3<Type> res (mat);
        res.inverse();
        return res;
    }
}
