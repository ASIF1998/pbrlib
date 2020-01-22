//
//  matrix4x4.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//
namespace pbrlib::math
{
    template<typename Type>
    Matrix4x4<Type>::Matrix4x4(Type init_value) 
    {
        for (size_t i{0}; i < 16; i++) {
            _linear_array[i] = init_value;
        }
    }

    template<typename Type>
    Matrix4x4<Type>::Matrix4x4(const Type* ptr_data) 
    {
        for (size_t i{0}; i < 16; i++) {
            _linear_array[i] = ptr_data[i];
        }
    }

    template<typename Type>
    Matrix4x4<Type>::Matrix4x4(Type v11, Type v12, Type v13, Type v14,
                               Type v21, Type v22, Type v23, Type v24,
                               Type v31, Type v32, Type v33, Type v34,
                               Type v41, Type v42, Type v43, Type v44) 
    {
        _two_dimensional_array[0][0] = v11;
        _two_dimensional_array[0][1] = v12;
        _two_dimensional_array[0][2] = v13;
        _two_dimensional_array[0][3] = v14;

        _two_dimensional_array[1][0] = v21;
        _two_dimensional_array[1][1] = v22;
        _two_dimensional_array[1][2] = v23;
        _two_dimensional_array[1][3] = v24;

        _two_dimensional_array[2][0] = v31;
        _two_dimensional_array[2][1] = v32;
        _two_dimensional_array[2][2] = v33;
        _two_dimensional_array[2][3] = v34;

        _two_dimensional_array[3][0] = v41;
        _two_dimensional_array[3][1] = v42;
        _two_dimensional_array[3][2] = v43;
        _two_dimensional_array[3][3] = v44;
    }

    template<typename Type>
    bool Matrix4x4<Type>::operator == (const Matrix4x4<Type>& mat) const
    {
        bool res = true;

        for (size_t i{0}; i < 16; i++) {
            res &= _linear_array[i] == mat._linear_array[i];
        }

        return res;
    }

    template<typename Type>
    bool Matrix4x4<Type>::operator != (const Matrix4x4<Type>& mat) const
    {
        bool res = true;

        for (size_t i{0}; i < 16; i++) {
            res |= _linear_array[i] != mat._linear_array[i];
        }

        return res;
    }

    template<typename Type>
    Matrix4x4<Type> Matrix4x4<Type>::operator + (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 16; i++) {
            res._linear_array[i] = _linear_array[i] + mat._linear_array[i];
        }

        return res;
    }

    template<typename Type>
    Matrix4x4<Type> Matrix4x4<Type>::operator - (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 16; i++) {
            res._linear_array[i] = _linear_array[i] - mat._linear_array[i];
        }

        return res;
    }

    template<typename Type>
    Matrix4x4<Type> Matrix4x4<Type>::operator * (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 4; i++) {
            for (size_t k{0}; k < 4; k++) {
                auto v = _two_dimensional_array[i][k];
                for (size_t j{0}; j < 4; j++) {
                    res._two_dimensional_array[i][j] += v * mat._two_dimensional_array[k][j];
                }
            }
        }

        return res;
    }

    template<typename Type>
    Matrix4x4<Type> Matrix4x4<Type>::operator * (Type scal) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 16; i++) {
            res._linear_array[i] = _linear_array[i] * scal;
        }

        return res;
    }

    template<typename Type>
    Matrix4x4<Type>& Matrix4x4<Type>::operator += (const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 16; i++) {
            _linear_array[i] += mat._linear_array[i];
        }

        return *this;
    }

    template<typename Type>
    Matrix4x4<Type>& Matrix4x4<Type>::operator -= (const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 16; i++) {
            _linear_array[i] -= mat._linear_array[i];
        }

        return *this;
    }

    template<typename Type>
    Matrix4x4<Type>& Matrix4x4<Type>::operator *= (const Matrix4x4<Type>& mat)
    {
        *this = *this * mat;
        return *this;
    }

    template<typename Type>
    Matrix4x4<Type>& Matrix4x4<Type>::operator *= (Type scal)
    {
        for (size_t i{0}; i < 16; i++) {
            _linear_array[i] *= scal;
        }

        return *this;
    }

    template<typename Type>
    inline Type* Matrix4x4<Type>::operator [] (size_t i)
    {
        return _two_dimensional_array[i];
    }

    template<typename Type>
    inline const Type* Matrix4x4<Type>::operator [] (size_t i) const
    {
        return _two_dimensional_array[i];
    }

    template<typename Type>
    Type& Matrix4x4<Type>::at(size_t i, size_t j)
    {
        assert(i < 4 && j < 4);
        return _two_dimensional_array[i][j];
    }

    template<typename Type>
    Type Matrix4x4<Type>::at(size_t i, size_t j) const
    {
        assert(i < 4 && j < 4);
        return _two_dimensional_array[i][j];
    }

    template<typename Type>
    Type Matrix4x4<Type>::det() const
    {
        auto a33444334 = _two_dimensional_array[2][2] * _two_dimensional_array[3][3] - _two_dimensional_array[3][2] * _two_dimensional_array[2][3];
        auto a32443442 = _two_dimensional_array[2][1] * _two_dimensional_array[3][3] - _two_dimensional_array[2][3] * _two_dimensional_array[3][1];
        auto a32433342 = _two_dimensional_array[2][1] * _two_dimensional_array[3][2] - _two_dimensional_array[2][2] * _two_dimensional_array[3][1];
        auto a31443441 = _two_dimensional_array[2][0] * _two_dimensional_array[3][3] - _two_dimensional_array[2][3] * _two_dimensional_array[3][0];
        auto a31433341 = _two_dimensional_array[2][0] * _two_dimensional_array[3][2] - _two_dimensional_array[2][2] * _two_dimensional_array[3][0];
        auto a31423241 = _two_dimensional_array[2][0] * _two_dimensional_array[3][1] - _two_dimensional_array[2][1] * _two_dimensional_array[3][0];

        auto s1 = _two_dimensional_array[0][0] * (_two_dimensional_array[1][1] * a33444334 - _two_dimensional_array[1][2] * a32443442 + _two_dimensional_array[1][3] * a32433342);
        auto s2 = _two_dimensional_array[0][1] * (_two_dimensional_array[1][0] * a33444334 - _two_dimensional_array[1][2] * a31443441 + _two_dimensional_array[1][3] * a31433341);
        auto s3 = _two_dimensional_array[0][2] * (_two_dimensional_array[1][0] * a32443442 - _two_dimensional_array[1][1] * a31443441 + _two_dimensional_array[1][3] * a31423241);
        auto s4 = _two_dimensional_array[0][3] * (_two_dimensional_array[1][0] * a32433342 - _two_dimensional_array[1][1] * a31433341 + _two_dimensional_array[1][2] * a31423241); 

        return s1 - s2 + s3 - s4;
    }

    template<typename Type>
    void Matrix4x4<Type>::transpose()
    {
        swap(_two_dimensional_array[0][1], _two_dimensional_array[1][0]);
        swap(_two_dimensional_array[0][2], _two_dimensional_array[2][0]);
        swap(_two_dimensional_array[0][3], _two_dimensional_array[3][0]);
        swap(_two_dimensional_array[1][2], _two_dimensional_array[2][1]);
        swap(_two_dimensional_array[1][3], _two_dimensional_array[3][1]);
        swap(_two_dimensional_array[2][3], _two_dimensional_array[3][2]);
    }

    template<typename Type>
    void Matrix4x4<Type>::inverse()
    {
        auto d = det();

        if (d != Type(0)) {
            auto tmat = pbrlib::math::transpose(*this);

            auto a33444334 = tmat[2][2] * tmat[3][3] - tmat[3][2] * tmat[2][3];
            auto a32443442 = tmat[2][1] * tmat[3][3] - tmat[2][3] * tmat[3][1];
            auto a32433342 = tmat[2][1] * tmat[3][2] - tmat[2][2] * tmat[3][1];
            auto a31443441 = tmat[2][0] * tmat[3][3] - tmat[2][3] * tmat[3][0];
            auto a31433341 = tmat[2][0] * tmat[3][2] - tmat[2][2] * tmat[3][0];
            auto a31423241 = tmat[2][0] * tmat[3][1] - tmat[2][1] * tmat[3][0];
            auto a23442443 = tmat[1][2] * tmat[3][3] - tmat[1][3] * tmat[3][2];
            auto a22444224 = tmat[1][1] * tmat[3][3] - tmat[3][1] * tmat[1][3];
            auto a22432342 = tmat[1][1] * tmat[3][2] - tmat[1][2] * tmat[3][1];
            auto a21442441 = tmat[1][0] * tmat[3][3] - tmat[1][3] * tmat[3][0];
            auto a21432341 = tmat[1][0] * tmat[3][2] - tmat[1][2] * tmat[3][0];
            auto a21422241 = tmat[1][0] * tmat[3][1] - tmat[1][1] * tmat[3][0];
            auto a23342433 = tmat[1][2] * tmat[2][3] - tmat[1][3] * tmat[2][2];
            auto a22342432 = tmat[1][1] * tmat[2][3] - tmat[1][3] * tmat[2][1];
            auto a22332332 = tmat[1][1] * tmat[2][2] - tmat[1][2] * tmat[2][1];
            auto a21342431 = tmat[1][0] * tmat[2][3] - tmat[1][3] * tmat[2][0];
            auto a21332331 = tmat[1][0] * tmat[2][2] - tmat[1][2] * tmat[2][0];
            auto a21322231 = tmat[1][0] * tmat[2][1] - tmat[1][1] * tmat[2][0];

            _two_dimensional_array[0][0] = tmat[1][1] * a33444334 - tmat[1][2] * a32443442 + tmat[1][3] * a32433342;
            _two_dimensional_array[0][1] = -(tmat[1][0] * a33444334 - tmat[1][2] * a31443441 + tmat[1][3] * a31433341);
            _two_dimensional_array[0][2] = tmat[1][0] * a32443442 - tmat[1][1] * a31443441 + tmat[1][3] * a31423241;
            _two_dimensional_array[0][3] = -(tmat[1][0] * a32433342 - tmat[1][1] * a31433341 + tmat[1][2] * a31423241);
            
            _two_dimensional_array[1][0] = -(tmat[0][1] * a33444334 - tmat[0][2] * a32443442 + tmat[0][3] * a32433342);
            _two_dimensional_array[1][1] =  tmat[0][0] * a33444334 - tmat[0][2] * a31443441 + tmat[0][3] * a31433341;
            _two_dimensional_array[1][2] = -(tmat[0][0] * a32443442 - tmat[0][1] * a31443441 + tmat[0][3] * a31423241);
            _two_dimensional_array[1][3] = tmat[0][0] * a32433342 - tmat[0][1] * a31433341 + tmat[0][2] * a31423241;
            
            _two_dimensional_array[2][0] = tmat[0][1] * a23442443 - tmat[0][2] * a22444224 + tmat[0][3] * a22432342;
            _two_dimensional_array[2][1] = -(tmat[0][0] * a23442443 - tmat[0][2] * a21442441 + tmat[0][3] * a21432341);
            _two_dimensional_array[2][2] = tmat[0][0] * a22444224 - tmat[0][1] * a21442441 + tmat[0][3] * a21422241;
            _two_dimensional_array[2][3] = -(tmat[0][0] * a22432342 - tmat[0][1] * a21432341 + tmat[0][2] * a21422241);
            
            _two_dimensional_array[3][0] = -(tmat[0][1] * a23342433 - tmat[0][2] * a22342432 + tmat[0][3] * a22332332);
            _two_dimensional_array[3][1] = tmat[0][0] * a23342433 - tmat[0][2] * a21342431 + tmat[0][3] * a21332331;
            _two_dimensional_array[3][2] = -(tmat[0][0] * a22342432 - tmat[0][1] * a21342431 + tmat[0][3] * a21322231);
            _two_dimensional_array[3][3] = tmat[0][0] * a22332332 - tmat[0][1] * a21332331 + tmat[0][2] * a21322231;
            
            *this *= (static_cast<Type>(1) / d);
        }
    }

    template<typename Type>
    Matrix4x4<Type> transpose(const Matrix4x4<Type>& mat)
    {
        return {
            mat[0][0], mat[1][0], mat[2][0], mat[3][0],
            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
            mat[0][3], mat[1][3], mat[2][3], mat[3][3]
        };
    }

    template<typename Type>
    Matrix4x4<Type> inverse(const Matrix4x4<Type>& mat)
    {
        Matrix4x4<Type> res (mat);
        res.inverse();
        return res;
    }

    template<typename Type>
    ostream& operator << (ostream& print, const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 4; i++) {
            print << mat[i][0] << ' ' << mat[i][1] << ' ' << mat[i][2] << ' ' << mat[i][3] << endl;
        }
        
        return print;
    }
}
