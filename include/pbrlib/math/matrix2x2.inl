#include <pbrlib/exceptions.hpp>

#include <format>

#include <memory>
#include <cassert>

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr Matrix2x2<T>::Matrix2x2() noexcept :
        _array4 
        {
            static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(1)
        }
    {}

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T>::Matrix2x2(T init_value) noexcept :
        _array4 
        {
            init_value, static_cast<T>(0),
            static_cast<T>(0), init_value
        }
    {}

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T>::Matrix2x2(
        T x0, T y0,
        T x1, T y1
    ) noexcept :
        _array4 
        {
            x0, y0,
            x1, y1
        }
    {}

    template<MathArithmetic T>
    inline constexpr bool Matrix2x2<T>::operator == (const Matrix2x2<T>& mat) const noexcept
    {
        return      _array4[0] == mat._array4[0] 
                &&  _array4[1] == mat._array4[1] 
                &&  _array4[2] == mat._array4[2] 
                &&  _array4[3] == mat._array4[3];
    }

    template<MathArithmetic T>
    inline constexpr bool Matrix2x2<T>::operator != (const Matrix2x2<T>& mat) const noexcept
    {
        return      _array4[0] != mat._array4[0] 
                ||  _array4[1] != mat._array4[1] 
                ||  _array4[2] != mat._array4[2] 
                ||  _array4[3] != mat._array4[3];
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> Matrix2x2<T>::operator + (const Matrix2x2<T>& mat) const noexcept
    {
        return 
        {
            _array4[0] + mat._array4[0],
            _array4[1] + mat._array4[1],
            _array4[2] + mat._array4[2],
            _array4[3] + mat._array4[3]
        };
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> Matrix2x2<T>::operator - (const Matrix2x2<T>& mat) const noexcept
    {
        return 
        {
            _array4[0] - mat._array4[0],
            _array4[1] - mat._array4[1],
            _array4[2] - mat._array4[2],
            _array4[3] - mat._array4[3]
        };
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> Matrix2x2<T>::operator * (const Matrix2x2<T>& mat) const noexcept
    {
        return 
        {
            _array2x2[0][0] * mat._array2x2[0][0] + _array2x2[1][0] * mat._array2x2[0][1],
            _array2x2[0][0] * mat._array2x2[1][0] + _array2x2[1][0] * mat._array2x2[1][1],
            _array2x2[0][1] * mat._array2x2[0][0] + _array2x2[1][1] * mat._array2x2[0][1],
            _array2x2[0][1] * mat._array2x2[1][0] + _array2x2[1][1] * mat._array2x2[1][1]
        };
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> operator * (const Matrix2x2<T>& mat, T s)
    {
        return 
        {
            mat[0][0] * s,
            mat[0][1] * s,
            mat[1][0] * s,
            mat[1][1] * s
        };
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> operator * (T s, const Matrix2x2<T>& mat)
    {
        return 
        {
            mat[0][0] * s,
            mat[0][1] * s,
            mat[1][0] * s,
            mat[1][1] * s
        };
    }

    template<MathArithmetic T>
    inline constexpr Vec2<T> Matrix2x2<T>::operator * (const Vec2<T>& v) const noexcept
    {
        return 
        {
            _array2x2[0][0] * v[0] + _array2x2[1][0] * v[1],
            _array2x2[0][1] * v[0] + _array2x2[1][1] * v[1]
        };
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T>& Matrix2x2<T>::operator += (const Matrix2x2<T>& mat) noexcept
    {
        _array4[0] += mat._array4[0];
        _array4[1] += mat._array4[1];
        _array4[2] += mat._array4[2];
        _array4[3] += mat._array4[3];

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T>& Matrix2x2<T>::operator -= (const Matrix2x2<T>& mat) noexcept
    {
        _array4[0] -= mat._array4[0];
        _array4[1] -= mat._array4[1];
        _array4[2] -= mat._array4[2];
        _array4[3] -= mat._array4[3];

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T>& Matrix2x2<T>::operator *= (const Matrix2x2<T>& mat) noexcept
    {
        auto temp (*this);

        _array4[0] = temp._array2x2[0][0] * mat._array2x2[0][0] + _array2x2[1][0] * mat._array2x2[0][1];
        _array4[1] = temp._array2x2[0][0] * mat._array2x2[1][0] + _array2x2[1][0] * mat._array2x2[1][1];
        _array4[2] = temp._array2x2[0][1] * mat._array2x2[0][0] + _array2x2[1][1] * mat._array2x2[0][1];
        _array4[3] = temp._array2x2[0][1] * mat._array2x2[1][0] + _array2x2[1][1] * mat._array2x2[1][1];

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T>& Matrix2x2<T>::operator *= (T scal) noexcept
    {
        _array4[0] *= scal;
        _array4[1] *= scal;
        _array4[2] *= scal;
        _array4[3] *= scal;

        return *this;
    }

    template<MathArithmetic T>
    inline T* Matrix2x2<T>::operator [] (size_t i) noexcept
    {
        return _array2x2[i];
    }

    template<MathArithmetic T>
    inline const T* Matrix2x2<T>::operator [] (size_t i) const noexcept
    {
        return _array2x2[i];
    }

    template<MathArithmetic T>
    inline T& Matrix2x2<T>::at(size_t i, size_t j)
    {
        if (i > 1 || j > 2) [[unlikely]] 
            throw exception::InvalidArgument(std::format("[math::mat2] i = {}, j = {}", i, j)); 

        return _array2x2[i][j];
    }

    template<MathArithmetic T>
    inline constexpr T Matrix2x2<T>::at(size_t i, size_t j) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct conditional access based on i and j
        return (i == 0 && j == 0) ? _array4[0] : 
               (i == 0 && j == 1) ? _array4[1] : 
               (i == 1 && j == 0) ? _array4[2] : _array4[3];
    }

    template<MathArithmetic T>
    inline constexpr T Matrix2x2<T>::det() const noexcept
    {
        return at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
    }

    template<MathArithmetic T>
    inline constexpr void Matrix2x2<T>::transpose() noexcept
    {
        auto temp = _array2x2[1][0];

        _array2x2[1][0] = _array2x2[0][1];
        _array2x2[0][1] = temp;
    }

    template<MathArithmetic T>
    inline void Matrix2x2<T>::inverse() noexcept
    {
        T d = det();

        if (d != static_cast<T>(0)) [[likely]] 
        {
            const T a11 = at(1, 1) / d;
            const T a12 = -at(0, 1) / d;
            const T a21 = -at(1, 0) / d;
            const T a22 = at(0, 0) / d;

            _array4[0] = a11;
            _array4[1] = a21;
            _array4[2] = a12;
            _array4[3] = a22;
        }
    }
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> transpose(const Matrix2x2<T>& mat) noexcept
    {
        return 
        {
            mat.at(0, 0), mat.at(1, 0),
            mat.at(0, 1), mat.at(1, 1)
        };
    }

    template<MathArithmetic T>
    inline constexpr Matrix2x2<T> inverse(const Matrix2x2<T>& mat) noexcept
    {
        T d = mat.det();

        if (d != static_cast<T>(0)) [[likely]] 
        {
            const T a11 = mat.at(1, 1) / d;
            const T a12 = -mat.at(0, 1) / d;
            const T a21 = -mat.at(1, 0) / d;
            const T a22 = mat.at(0, 0) / d;

            return Matrix2x2<T>(a11, a21, a12, a22);
        }

        return mat;
    }

    template<MathArithmetic T>
    inline Matrix2x2<T> round(const Matrix2x2<T>& mat) noexcept
    {
        const Matrix2x2<T> res (
            std::round(mat.at(0, 0)), std::round(mat.at(0, 1)),
            std::round(mat.at(1, 0)), std::round(mat.at(1, 1))
        );

        return res;
    }
}
