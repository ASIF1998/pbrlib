#include <pbrlib/exceptions.hpp>

#include <format>

#include <memory>
#include <cassert>

namespace pbrlib::math
{
    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type>::Matrix2x2() noexcept :
        _array4 
        {
            static_cast<Type>(1), static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(1)
        }
    {}

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type>::Matrix2x2(Type init_value) noexcept :
        _array4 
        {
            init_value, static_cast<Type>(0),
            static_cast<Type>(0), init_value
        }
    {}

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type>::Matrix2x2(
        Type x0, Type y0,
        Type x1, Type y1
    ) noexcept :
        _array4 
        {
            x0, y0,
            x1, y1
        }
    {}

    template<MathArithmetic Type>
    inline constexpr bool Matrix2x2<Type>::operator == (const Matrix2x2<Type>& mat) const noexcept
    {
        return      _array4[0] == mat._array4[0] 
                &&  _array4[1] == mat._array4[1] 
                &&  _array4[2] == mat._array4[2] 
                &&  _array4[3] == mat._array4[3];
    }

    template<MathArithmetic Type>
    inline constexpr bool Matrix2x2<Type>::operator != (const Matrix2x2<Type>& mat) const noexcept
    {
        return      _array4[0] != mat._array4[0] 
                ||  _array4[1] != mat._array4[1] 
                ||  _array4[2] != mat._array4[2] 
                ||  _array4[3] != mat._array4[3];
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type> Matrix2x2<Type>::operator + (const Matrix2x2<Type>& mat) const noexcept
    {
        return 
        {
            _array4[0] + mat._array4[0],
            _array4[1] + mat._array4[1],
            _array4[2] + mat._array4[2],
            _array4[3] + mat._array4[3]
        };
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type> Matrix2x2<Type>::operator - (const Matrix2x2<Type>& mat) const noexcept
    {
        return 
        {
            _array4[0] - mat._array4[0],
            _array4[1] - mat._array4[1],
            _array4[2] - mat._array4[2],
            _array4[3] - mat._array4[3]
        };
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type> Matrix2x2<Type>::operator * (const Matrix2x2<Type>& mat) const noexcept
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

    template<MathArithmetic Type>
    inline constexpr Vec2<Type> Matrix2x2<Type>::operator * (const Vec2<Type>& v) const noexcept
    {
        return 
        {
            _array2x2[0][0] * v[0] + _array2x2[1][0] * v[1],
            _array2x2[0][1] * v[0] + _array2x2[1][1] * v[1]
        };
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type>& Matrix2x2<Type>::operator += (const Matrix2x2<Type>& mat) noexcept
    {
        _array4[0] += mat._array4[0];
        _array4[1] += mat._array4[1];
        _array4[2] += mat._array4[2];
        _array4[3] += mat._array4[3];

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type>& Matrix2x2<Type>::operator -= (const Matrix2x2<Type>& mat) noexcept
    {
        _array4[0] -= mat._array4[0];
        _array4[1] -= mat._array4[1];
        _array4[2] -= mat._array4[2];
        _array4[3] -= mat._array4[3];

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type>& Matrix2x2<Type>::operator *= (const Matrix2x2<Type>& mat) noexcept
    {
        auto temp (*this);

        _array4[0] = temp._array2x2[0][0] * mat._array2x2[0][0] + _array2x2[1][0] * mat._array2x2[0][1];
        _array4[1] = temp._array2x2[0][0] * mat._array2x2[1][0] + _array2x2[1][0] * mat._array2x2[1][1];
        _array4[2] = temp._array2x2[0][1] * mat._array2x2[0][0] + _array2x2[1][1] * mat._array2x2[0][1];
        _array4[3] = temp._array2x2[0][1] * mat._array2x2[1][0] + _array2x2[1][1] * mat._array2x2[1][1];

        return *this;
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type>& Matrix2x2<Type>::operator *= (Type scal) noexcept
    {
        _array4[0] *= scal;
        _array4[1] *= scal;
        _array4[2] *= scal;
        _array4[3] *= scal;

        return *this;
    }

    template<MathArithmetic Type>
    inline Type* Matrix2x2<Type>::operator [] (size_t i) noexcept
    {
        return _array2x2[i];
    }

    template<MathArithmetic Type>
    inline const Type* Matrix2x2<Type>::operator [] (size_t i) const noexcept
    {
        return _array2x2[i];
    }

    template<MathArithmetic Type>
    inline Type& Matrix2x2<Type>::at(size_t i, size_t j)
    {
        if (i > 1 || j > 2) [[unlikely]] 
            throw exception::InvalidArgument(std::format("[math::mat2] i = {}, j = {}", i, j)); 

        return _array2x2[i][j];
    }

    template<MathArithmetic Type>
    inline constexpr Type Matrix2x2<Type>::at(size_t i, size_t j) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct conditional access based on i and j
        return (i == 0 && j == 0) ? _array4[0] : 
               (i == 0 && j == 1) ? _array4[1] : 
               (i == 1 && j == 0) ? _array4[2] : _array4[3];
    }

    template<MathArithmetic Type>
    inline constexpr Type Matrix2x2<Type>::det() const noexcept
    {
        return at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
    }

    template<MathArithmetic Type>
    inline constexpr void Matrix2x2<Type>::transpose() noexcept
    {
        auto temp = _array2x2[1][0];

        _array2x2[1][0] = _array2x2[0][1];
        _array2x2[0][1] = temp;
    }

    template<MathArithmetic Type>
    inline void Matrix2x2<Type>::inverse() noexcept
    {
        Type d = det();

        if (d != static_cast<Type>(0)) [[likely]] 
        {
            const Type a11 = at(1, 1) / d;
            const Type a12 = -at(0, 1) / d;
            const Type a21 = -at(1, 0) / d;
            const Type a22 = at(0, 0) / d;

            _array4[0] = a11;
            _array4[1] = a21;
            _array4[2] = a12;
            _array4[3] = a22;
        }
    }
}

namespace pbrlib::math
{
    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type> transpose(const Matrix2x2<Type>& mat) noexcept
    {
        return 
        {
            mat.at(0, 0), mat.at(1, 0),
            mat.at(0, 1), mat.at(1, 1)
        };
    }

    template<MathArithmetic Type>
    inline constexpr Matrix2x2<Type> inverse(const Matrix2x2<Type>& mat) noexcept
    {
        Type d = mat.det();

        if (d != static_cast<Type>(0)) [[likely]] 
        {
            const Type a11 = mat.at(1, 1) / d;
            const Type a12 = -mat.at(0, 1) / d;
            const Type a21 = -mat.at(1, 0) / d;
            const Type a22 = mat.at(0, 0) / d;

            return Matrix2x2<Type>(a11, a21, a12, a22);
        }

        return mat;
    }

    template<MathArithmetic Type>
    inline Matrix2x2<Type> round(const Matrix2x2<Type>& mat) noexcept
    {
        const Matrix2x2<Type> res (
            std::round(mat.at(0, 0)), std::round(mat.at(0, 1)),
            std::round(mat.at(1, 0)), std::round(mat.at(1, 1))
        );

        return res;
    }
}
