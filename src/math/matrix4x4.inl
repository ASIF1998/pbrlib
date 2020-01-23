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

        auto s1 = _two_dimensional_array[0][0] * (_two_dimensional_array[1][1] * a33444334 - 
                                                  _two_dimensional_array[1][2] * a32443442 + 
                                                  _two_dimensional_array[1][3] * a32433342);

        auto s2 = _two_dimensional_array[0][1] * (_two_dimensional_array[1][0] * a33444334 - 
                                                  _two_dimensional_array[1][2] * a31443441 + 
                                                  _two_dimensional_array[1][3] * a31433341);

        auto s3 = _two_dimensional_array[0][2] * (_two_dimensional_array[1][0] * a32443442 - 
                                                  _two_dimensional_array[1][1] * a31443441 + 
                                                  _two_dimensional_array[1][3] * a31423241);

        auto s4 = _two_dimensional_array[0][3] * (_two_dimensional_array[1][0] * a32433342 -
                                                  _two_dimensional_array[1][1] * a31433341 + 
                                                  _two_dimensional_array[1][2] * a31423241); 

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

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Matrix4x4<float>::Matrix4x4(float init_value)
    {
        for (size_t i{0}; i < 16; i++) {
            _linear_array[i] = init_value;
        }
    }

    Matrix4x4<float>::Matrix4x4(float v11, float v12, float v13, float v14,
                                float v21, float v22, float v23, float v24,
                                float v31, float v32, float v33, float v34,
                                float v41, float v42, float v43, float v44)
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

    Matrix4x4<float>::Matrix4x4(const float* ptr_data)
    {
        for (size_t i{0}; i < 16; i++) {
            _linear_array[i] = ptr_data[i];
        }
    }

    Matrix4x4<float>::Matrix4x4(const __m256& s1, const __m256& s2)
    {
        _m256_simd[0] = s1;
        _m256_simd[1] = s2;
    }

    bool Matrix4x4<float>::operator == (const Matrix4x4<float>& mat) const
    {
        auto r1 = _mm_cmpeq_ps(_m128_simd[0], mat._m128_simd[0]);
        auto r2 = _mm_cmpeq_ps(_m128_simd[1], mat._m128_simd[1]);
        auto r3 = _mm_cmpeq_ps(_m128_simd[2], mat._m128_simd[2]);
        auto r4 = _mm_cmpeq_ps(_m128_simd[3], mat._m128_simd[3]);
        return _mm_movemask_ps(_mm_and_ps(_mm_and_ps(r1, r2), _mm_and_ps(r3, r4))) == 15;
    }

    bool Matrix4x4<float>::operator != (const Matrix4x4<float>& mat) const
    {
        return !(*this == mat);
    }

    Matrix4x4<float> Matrix4x4<float>::operator + (const Matrix4x4<float>& mat) const
    {
        return {
            _mm256_add_ps(_m256_simd[0], mat._m256_simd[0]),
            _mm256_add_ps(_m256_simd[1], mat._m256_simd[1])
        };
    }

    Matrix4x4<float> Matrix4x4<float>::operator - (const Matrix4x4<float>& mat) const
    {
        return {
            _mm256_sub_ps(_m256_simd[0], mat._m256_simd[0]),
            _mm256_sub_ps(_m256_simd[1], mat._m256_simd[1])
        };
    }

    Matrix4x4<float> Matrix4x4<float>::operator * (const Matrix4x4<float>& mat) const
    {
        auto a21 = _mm256_setr_m128(_m128_simd[1], _m128_simd[0]);
        auto a43 = _mm256_setr_m128(_m128_simd[3], _m128_simd[2]);
        auto b12 = _mm256_setr_ps(mat[0][0], mat[1][0], mat[2][0], mat[3][0], mat[0][1], mat[1][1], mat[2][1], mat[3][1]);
        auto b34 = _mm256_setr_ps(mat[0][2], mat[1][2], mat[2][2], mat[3][2], mat[0][3], mat[1][3], mat[2][3], mat[3][3]);

        auto lc1122 = _mm256_mul_ps(_m256_simd[0], b12);
        auto lc2112 = _mm256_mul_ps(a21, b12);
        auto lc1324 = _mm256_mul_ps(_m256_simd[0], b34);
        auto lc2314 = _mm256_mul_ps(a21, b34);
        auto lc3142 = _mm256_mul_ps(_m256_simd[1], b12);
        auto lc4132 = _mm256_mul_ps(a43, b12);
        auto lc3344 = _mm256_mul_ps(_m256_simd[1], b34);
        auto lc4334 = _mm256_mul_ps(a43, b34);

        return {
            lc1122[0] + lc1122[1] + lc1122[2] + lc1122[3], 
            lc2112[4] + lc2112[5] + lc2112[6] + lc2112[7], 
            lc1324[0] + lc1324[1] + lc1324[2] + lc1324[3], 
            lc2314[4] + lc2314[5] + lc2314[6] + lc2314[7],
            lc2112[0] + lc2112[1] + lc2112[2] + lc2112[3], 
            lc1122[4] + lc1122[5] + lc1122[6] + lc1122[7], 
            lc2314[0] + lc2314[1] + lc2314[2] + lc2314[3], 
            lc1324[4] + lc1324[5] + lc1324[6] + lc1324[7],
            lc3142[0] + lc3142[1] + lc3142[2] + lc3142[3], 
            lc4132[4] + lc4132[5] + lc4132[6] + lc4132[7], 
            lc3344[0] + lc3344[1] + lc3344[2] + lc3344[3], 
            lc4334[4] + lc4334[5] + lc4334[6] + lc4334[7],
            lc4132[0] + lc4132[1] + lc4132[2] + lc4132[3], 
            lc3142[4] + lc3142[5] + lc3142[6] + lc3142[7], 
            lc4334[0] + lc4334[1] + lc4334[2] + lc4334[3], 
            lc3344[4] + lc3344[5] + lc3344[6] + lc3344[7]
        };
    }

    Matrix4x4<float> Matrix4x4<float>::operator * (float scal) const
    {
        auto vscal = _mm256_set1_ps(scal);

        return {
            _mm256_mul_ps(_m256_simd[0], vscal),
            _mm256_mul_ps(_m256_simd[1], vscal)
        };
    }

    Matrix4x4<float>& Matrix4x4<float>::operator += (const Matrix4x4<float>& mat) 
    {
        _m256_simd[0] = _mm256_add_ps(_m256_simd[0], mat._m256_simd[0]);
        _m256_simd[1] = _mm256_add_ps(_m256_simd[1], mat._m256_simd[1]);
        return *this;
    }

    Matrix4x4<float>& Matrix4x4<float>::operator -= (const Matrix4x4<float>& mat) 
    {
        _m256_simd[0] = _mm256_sub_ps(_m256_simd[0], mat._m256_simd[0]);
        _m256_simd[1] = _mm256_sub_ps(_m256_simd[1], mat._m256_simd[1]);
        return *this;
    }

    Matrix4x4<float>& Matrix4x4<float>::operator *= (const Matrix4x4<float>& mat) 
    {
        auto a21 = _mm256_setr_m128(_m128_simd[1], _m128_simd[0]);
        auto a43 = _mm256_setr_m128(_m128_simd[3], _m128_simd[2]);
        auto b12 = _mm256_setr_ps(mat[0][0], mat[1][0], mat[2][0], mat[3][0], mat[0][1], mat[1][1], mat[2][1], mat[3][1]);
        auto b34 = _mm256_setr_ps(mat[0][2], mat[1][2], mat[2][2], mat[3][2], mat[0][3], mat[1][3], mat[2][3], mat[3][3]);

        auto lc1122 = _mm256_mul_ps(_m256_simd[0], b12);
        auto lc2112 = _mm256_mul_ps(a21, b12);
        auto lc1324 = _mm256_mul_ps(_m256_simd[0], b34);
        auto lc2314 = _mm256_mul_ps(a21, b34);
        auto lc3142 = _mm256_mul_ps(_m256_simd[1], b12);
        auto lc4132 = _mm256_mul_ps(a43, b12);
        auto lc3344 = _mm256_mul_ps(_m256_simd[1], b34);
        auto lc4334 = _mm256_mul_ps(a43, b34);
        
        _two_dimensional_array[0][0] = lc1122[0] + lc1122[1] + lc1122[2] + lc1122[3];
        _two_dimensional_array[0][1] = lc2112[4] + lc2112[5] + lc2112[6] + lc2112[7];
        _two_dimensional_array[0][2] = lc1324[0] + lc1324[1] + lc1324[2] + lc1324[3];
        _two_dimensional_array[0][3] = lc2314[4] + lc2314[5] + lc2314[6] + lc2314[7];
        _two_dimensional_array[1][0] = lc2112[0] + lc2112[1] + lc2112[2] + lc2112[3];
        _two_dimensional_array[1][1] = lc1122[4] + lc1122[5] + lc1122[6] + lc1122[7];
        _two_dimensional_array[1][2] = lc2314[0] + lc2314[1] + lc2314[2] + lc2314[3];
        _two_dimensional_array[1][3] = lc1324[4] + lc1324[5] + lc1324[6] + lc1324[7];
        _two_dimensional_array[2][0] = lc3142[0] + lc3142[1] + lc3142[2] + lc3142[3];
        _two_dimensional_array[2][1] = lc4132[4] + lc4132[5] + lc4132[6] + lc4132[7];
        _two_dimensional_array[2][2] = lc3344[0] + lc3344[1] + lc3344[2] + lc3344[3];
        _two_dimensional_array[2][3] = lc4334[4] + lc4334[5] + lc4334[6] + lc4334[7];
        _two_dimensional_array[3][0] = lc4132[0] + lc4132[1] + lc4132[2] + lc4132[3];
        _two_dimensional_array[3][1] = lc3142[4] + lc3142[5] + lc3142[6] + lc3142[7];
        _two_dimensional_array[3][2] = lc4334[0] + lc4334[1] + lc4334[2] + lc4334[3];
        _two_dimensional_array[3][3] = lc3344[4] + lc3344[5] + lc3344[6] + lc3344[7];
        
        return *this;
    }

    Matrix4x4<float>& Matrix4x4<float>::operator *= (float scal)
    {
        auto vscal = _mm256_set1_ps(scal);

        _m256_simd[0] = _mm256_mul_ps(_m256_simd[0], vscal);
        _m256_simd[1] = _mm256_mul_ps(_m256_simd[1], vscal);
        return *this;
    }

    inline float* Matrix4x4<float>::operator [] (size_t i) noexcept
    {
        return _two_dimensional_array[i];
    }

    inline const float* Matrix4x4<float>::operator [] (size_t i) const noexcept
    {
        return _two_dimensional_array[i];
    }

    inline float& Matrix4x4<float>::at(size_t i, size_t j) 
    {
        assert(i < 4 && j < 4);
        return _two_dimensional_array[i][j];
    }

    inline float Matrix4x4<float>::at(size_t i, size_t j) const
    {
        assert(i < 4 && j < 4);
        return _two_dimensional_array[i][j];
    }

    float Matrix4x4<float>::det() const
    {
        float a33444334 = _two_dimensional_array[2][2] * _two_dimensional_array[3][3] - _two_dimensional_array[3][2] * _two_dimensional_array[2][3];
        float a32443442 = _two_dimensional_array[2][1] * _two_dimensional_array[3][3] - _two_dimensional_array[2][3] * _two_dimensional_array[3][1];
        float a32433342 = _two_dimensional_array[2][1] * _two_dimensional_array[3][2] - _two_dimensional_array[2][2] * _two_dimensional_array[3][1];
        float a31443441 = _two_dimensional_array[2][0] * _two_dimensional_array[3][3] - _two_dimensional_array[2][3] * _two_dimensional_array[3][0];
        float a31433341 = _two_dimensional_array[2][0] * _two_dimensional_array[3][2] - _two_dimensional_array[2][2] * _two_dimensional_array[3][0];
        float a31423241 = _two_dimensional_array[2][0] * _two_dimensional_array[3][1] - _two_dimensional_array[2][1] * _two_dimensional_array[3][0];

        float s1 = _two_dimensional_array[0][0] * (_two_dimensional_array[1][1] * a33444334 - 
                                                  _two_dimensional_array[1][2] * a32443442 + 
                                                  _two_dimensional_array[1][3] * a32433342);

        float s2 = _two_dimensional_array[0][1] * (_two_dimensional_array[1][0] * a33444334 - 
                                                  _two_dimensional_array[1][2] * a31443441 + 
                                                  _two_dimensional_array[1][3] * a31433341);

        float s3 = _two_dimensional_array[0][2] * (_two_dimensional_array[1][0] * a32443442 - 
                                                  _two_dimensional_array[1][1] * a31443441 + 
                                                  _two_dimensional_array[1][3] * a31423241);

        float s4 = _two_dimensional_array[0][3] * (_two_dimensional_array[1][0] * a32433342 -
                                                  _two_dimensional_array[1][1] * a31433341 + 
                                                  _two_dimensional_array[1][2] * a31423241); 

        return s1 - s2 + s3 - s4;
    }

    void Matrix4x4<float>::transpose()
    {
        swap(_two_dimensional_array[0][1], _two_dimensional_array[1][0]);
        swap(_two_dimensional_array[0][2], _two_dimensional_array[2][0]);
        swap(_two_dimensional_array[0][3], _two_dimensional_array[3][0]);
        swap(_two_dimensional_array[1][2], _two_dimensional_array[2][1]);
        swap(_two_dimensional_array[1][3], _two_dimensional_array[3][1]);
        swap(_two_dimensional_array[2][3], _two_dimensional_array[3][2]);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
