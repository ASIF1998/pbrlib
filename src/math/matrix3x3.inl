//
//  matrix3x3.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 30/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <memory>

#include <algorithm>

using namespace std;

namespace pbrlib::math
{
    template<typename Type>
    Matrix3x3<Type>::Matrix3x3(Type init_value)
    {
        for (size_t i{0}; i < 9; i++) {
            _array9[i] = init_value;
        }
    }

    template<typename Type>
    Matrix3x3<Type>::Matrix3x3(Type v11, Type v12, Type v13,
                               Type v21, Type v22, Type v23,
                               Type v31, Type v32, Type v33)
    {
        _array3x3[0][0] = v11;
        _array3x3[0][1] = v12;
        _array3x3[0][2] = v13;

        _array3x3[1][0] = v21;
        _array3x3[1][1] = v22;
        _array3x3[1][2] = v23;

        _array3x3[2][0] = v31;
        _array3x3[2][1] = v32;
        _array3x3[2][2] = v33;
    }

    template<typename Type>
    inline Matrix3x3<Type>::Matrix3x3(const Type* ptr_data)
    {
        memcpy(_array9, ptr_data, 9 * sizeof(Type));
    }

    template<typename Type>
    bool Matrix3x3<Type>::operator == (const Matrix3x3<Type>& mat) const
    {
        bool res = true;

        for (size_t i{0}; i < 9; i++) {
            res &= _array9[i] == mat._array9[i];
        }

        return res;
    }

    template<typename Type>
    bool Matrix3x3<Type>::operator != (const Matrix3x3<Type>& mat) const
    {
        bool res = true;

        for (size_t i{0}; i < 9; i++) {
            res |= _array9[i] != mat._array9[i];
        }

        return res;
    }

    template<typename Type>
    Matrix3x3<Type> Matrix3x3<Type>::operator + (const Matrix3x3<Type>& mat) const
    {
        Matrix3x3<Type> res;

        for (size_t i{0}; i < 9; i++) {
            res._linear_array[i] = _array9[i] + mat._array9[i];
        }

        return res;
    }

    template<typename Type>
    Matrix3x3<Type> Matrix3x3<Type>::operator * (const Matrix3x3<Type>& mat) const
    {
        Matrix3x3<Type> res;

        for (size_t i{0}; i < 3; i++) {
            for (size_t k{0}; k < 3; k++) {
                auto v = _array3x3[i][k];
                for (size_t j{0}; j < 3; j++) {
                    res._array3x3[i][j] += v * mat._array3x3[k][j];
                }
            }
        }

        return res;
    }

    template<typename Type>
    Matrix3x3<Type> Matrix3x3<Type>::operator * (Type scal) const
    {
        Matrix3x3<Type> res;

        for (size_t i{0}; i < 9; i++) {
            res._array9[i] = _array9[i] * scal;
        }

        return res;
    }

    template<typename Type>
    Vec3<Type> Matrix3x3<Type>::operator * (const Vec3<Type>& v) const
    {
        Vec3<Type> res;

        for (size_t i{0}; i < 3; i++) {
            res.x += _array3x3[0][i] * v[i];
            res.y += _array3x3[1][i] * v[i];
            res.z += _array3x3[2][i] * v[i];
        }

        return res;
    }

    template<typename Type>
    Matrix3x3<Type>& Matrix3x3<Type>::operator += (const Matrix3x3<Type>& mat)
    {
        for (size_t i{0}; i < 9; i++) {
            _array9[i] += mat._array9[i];
        }

        return *this;
    }

    template<typename Type>
    Matrix3x3<Type>& Matrix3x3<Type>::operator -= (const Matrix3x3<Type>& mat)
    {
        for (size_t i{0}; i < 9; i++) {
            _array9[i] += mat._array9[i];
        }

        return *this;
    }

    template<typename Type>
    Matrix3x3<Type>& Matrix3x3<Type>::operator *= (const Matrix3x3<Type>& mat)
    {
        *this = *this * mat;
        return *this;
    }

    template<typename Type>
    Matrix3x3<Type>& Matrix3x3<Type>::operator *= (Type scal)
    {
        for (size_t i{0}; i < 9; i++) {
            _array9[i] *= scal;
        }

        return *this;
    }

    template<typename Type>
    inline Type* Matrix3x3<Type>::operator [] (size_t i)
    {
        return _array3x3[i];
    }

    template<typename Type>
    inline const Type* Matrix3x3<Type>::operator [] (size_t i) const
    {
        return _array3x3[i];
    }

    template<typename Type>
    Type& Matrix3x3<Type>::at(size_t i, size_t j)
    {
        assert(i < 3 && j < 3);
        return _array3x3[i][j];
    }

    template<typename Type>
    Type Matrix3x3<Type>::at(size_t i, size_t j) const
    {
        assert(i < 3 && j < 3);
        return _array3x3[i][j];
    }

    template<typename Type>
    Type Matrix3x3<Type>::det() const
    {
        return  (_array3x3[0][0] * _array3x3[1][1] * _array3x3[2][2]) + 
                (_array3x3[2][0] * _array3x3[0][1] * _array3x3[1][2]) + 
                (_array3x3[1][0] * _array3x3[2][1] * _array3x3[0][2]) -
                (_array3x3[2][0] * _array3x3[1][1] * _array3x3[0][2]) - 
                (_array3x3[0][0] * _array3x3[2][1] * _array3x3[1][2]) - 
                (_array3x3[1][0] * _array3x3[0][1] * _array3x3[2][2]);
    }

    template<typename Type>
    void Matrix3x3<Type>::transpose()
    {
        swap(_array3x3[0][1], _array3x3[1][0]);
        swap(_array3x3[0][2], _array3x3[2][0]);
        swap(_array3x3[1][2], _array3x3[2][1]);
    }

    template<typename Type>
    void Matrix3x3<Type>::inverse()
    {
        auto d = det();

        if (d != Type(0)) {
            auto tmat (*this);

            tmat.transpose();

            _array3x3[0][0] = (tmat._array3x3[1][1] * tmat._array3x3[2][2] - tmat._array3x3[1][2] * tmat._array3x3[2][1]);
            _array3x3[0][1] = -(tmat._array3x3[1][0] * tmat._array3x3[2][2] - tmat._array3x3[1][2] * tmat._array3x3[2][0]);
            _array3x3[0][2] = (tmat._array3x3[1][0] * tmat._array3x3[2][1] - tmat._array3x3[1][1] * tmat._array3x3[2][0]);

            _array3x3[1][0] = -(tmat._array3x3[0][1] * tmat._array3x3[2][2] - tmat._array3x3[0][2] * tmat._array3x3[2][1]);
            _array3x3[1][1] = (tmat._array3x3[0][0] * tmat._array3x3[2][2] - tmat._array3x3[0][2] * tmat._array3x3[2][0]);
            _array3x3[1][2] = -(tmat._array3x3[0][0] * tmat._array3x3[2][1] - tmat._array3x3[0][1] * tmat._array3x3[2][0]);

            _array3x3[2][0] = (tmat._array3x3[0][1] * tmat._array3x3[1][2] - tmat._array3x3[0][2] * tmat._array3x3[1][1]);
            _array3x3[2][1] = -(tmat._array3x3[0][0] * tmat._array3x3[1][2] - tmat._array3x3[0][2] * tmat._array3x3[1][0]);
            _array3x3[2][2] = (tmat._array3x3[0][0] * tmat._array3x3[1][1] - tmat._array3x3[0][1] * tmat._array3x3[1][0]);

            *this *= (static_cast<Type>(1) / d);
        }
    }

    template<typename Type>
    Matrix3x3<Type> transpose(const Matrix3x3<Type>& mat)
    {
        return {
            mat[0][0], mat[1][0], mat[2][0],
            mat[0][1], mat[1][1], mat[2][1],
            mat[0][2], mat[1][2], mat[2][2]
        };
    }

    template<typename Type>
    Matrix3x3<Type> inverse(const Matrix3x3<Type>& mat)
    {
        Matrix3x3<Type> res (mat);
        res.inverse();
        return res;
    }

    template<typename Type>
    ostream& operator << (ostream& print, const Matrix3x3<Type>& mat)
    {
        for (size_t i{0}; i < 3; i++) {
            print << mat[i][0] << ' ' << mat[i][1] << ' ' << mat[i][2] << endl;
        }
        
        return print;
    }
}
