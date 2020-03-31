//
//  matrix4x4.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 22/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <memory>

#include "vec4.hpp"

namespace pbrlib::math
{
    template<typename Type>
    inline constexpr Matrix4x4<Type>::Matrix4x4() :
        _array16 {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        }
    {}

    template<typename Type>
    inline constexpr Matrix4x4<Type>::Matrix4x4(Type init_value) :
        _array16 {
            init_value, init_value, init_value, init_value,
            init_value, init_value, init_value, init_value,
            init_value, init_value, init_value, init_value,
            init_value, init_value, init_value, init_value
        }
    {}

    template<typename Type>
    inline constexpr Matrix4x4<Type>::Matrix4x4(
        Type v11, Type v12, Type v13, Type v14,
        Type v21, Type v22, Type v23, Type v24,
        Type v31, Type v32, Type v33, Type v34, 
        Type v41, Type v42, Type v43, Type v44
    ) :
        _array16 {
            v11, v12, v13, v14,
            v21, v22, v23, v24,
            v31, v32, v33, v34,
            v41, v42, v43, v44,
        }
    {}

    template<typename Type>
    inline Matrix4x4<Type>::Matrix4x4(const Type* ptr_data) 
    {
        assert(ptr_data);
        memcpy(_array16, ptr_data, sizeof(Type) * 16);
    }

    template<typename Type>
    inline bool Matrix4x4<Type>::operator == (const Matrix4x4<Type>& mat) const
    {
        bool res = true;

        for (size_t i{0}; i < 16; i++) {
            res &= _array16[i] == mat._array16[i];
        }

        return res;
    }

    template<typename Type>
    inline bool Matrix4x4<Type>::operator != (const Matrix4x4<Type>& mat) const
    {
        bool res = true;

        for (size_t i{0}; i < 16; i++) {
            res |= _array16[i] != mat._array16[i];
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type> Matrix4x4<Type>::operator + (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 16; i++) {
            res._array16[i] = _array16[i] + mat._array16[i];
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type> Matrix4x4<Type>::operator - (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 16; i++) {
            res._array16[i] = _array16[i] - mat._array16[i];
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type> Matrix4x4<Type>::operator * (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res (static_cast<Type>(0));

        for (size_t i{0}; i < 4; i++) {
            for (size_t k{0}; k < 4; k++) {
                auto v = _array4x4[i][k];
                for (size_t j{0}; j < 4; j++) {
                    res._array4x4[i][j] += v * mat._array4x4[k][j];
                }
            }
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type> Matrix4x4<Type>::operator * (Type scal) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 16; i++) {
            res._array16[i] = _array16[i] * scal;
        }

        return res;
    }

    template<typename Type>
    inline Vec4<Type> Matrix4x4<Type>::operator * (const Vec4<Type>& v) const
    {
        Vec4<Type> res;

        for (size_t i{0}; i < 4; i++) {
            res.x += _array4x4[0][i] * v[i];
            res.y += _array4x4[1][i] * v[i];
            res.z += _array4x4[2][i] * v[i];
            res.w += _array4x4[3][i] * v[i];
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type>& Matrix4x4<Type>::operator += (const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 16; i++) {
            _array16[i] += mat._array16[i];
        }

        return *this;
    }

    template<typename Type>
    inline Matrix4x4<Type>& Matrix4x4<Type>::operator -= (const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 16; i++) {
            _array16[i] -= mat._array16[i];
        }

        return *this;
    }

    template<typename Type>
    inline Matrix4x4<Type>& Matrix4x4<Type>::operator *= (const Matrix4x4<Type>& mat)
    {
        *this = *this * mat;
        return *this;
    }

    template<typename Type>
    inline Matrix4x4<Type>& Matrix4x4<Type>::operator *= (Type scal)
    {
        for (size_t i{0}; i < 16; i++) {
            _array16[i] *= scal;
        }

        return *this;
    }

    template<typename Type>
    inline Type* Matrix4x4<Type>::operator [] (size_t i)
    {
        return _array4x4[i];
    }

    template<typename Type>
    inline const Type* Matrix4x4<Type>::operator [] (size_t i) const
    {
        return _array4x4[i];
    }

    template<typename Type>
    inline Type& Matrix4x4<Type>::at(size_t i, size_t j)
    {
        assert(i < 4 && j < 4);
        return _array4x4[i][j];
    }

    template<typename Type>
    inline Type Matrix4x4<Type>::at(size_t i, size_t j) const
    {
        assert(i < 4 && j < 4);
        return _array4x4[i][j];
    }

    template<typename Type>
    inline Type Matrix4x4<Type>::det() const
    {
        auto a33444334 = _array4x4[2][2] * _array4x4[3][3] - _array4x4[3][2] * _array4x4[2][3];
        auto a32443442 = _array4x4[2][1] * _array4x4[3][3] - _array4x4[2][3] * _array4x4[3][1];
        auto a32433342 = _array4x4[2][1] * _array4x4[3][2] - _array4x4[2][2] * _array4x4[3][1];
        auto a31443441 = _array4x4[2][0] * _array4x4[3][3] - _array4x4[2][3] * _array4x4[3][0];
        auto a31433341 = _array4x4[2][0] * _array4x4[3][2] - _array4x4[2][2] * _array4x4[3][0];
        auto a31423241 = _array4x4[2][0] * _array4x4[3][1] - _array4x4[2][1] * _array4x4[3][0];

        auto s1 = _array4x4[0][0] * (
            _array4x4[1][1] * a33444334 -
            _array4x4[1][2] * a32443442 +
            _array4x4[1][3] * a32433342
        );

        auto s2 = _array4x4[0][1] * (
            _array4x4[1][0] * a33444334 -
            _array4x4[1][2] * a31443441 +
            _array4x4[1][3] * a31433341
        );

        auto s3 = _array4x4[0][2] * (
            _array4x4[1][0] * a32443442 -
            _array4x4[1][1] * a31443441 +
            _array4x4[1][3] * a31423241
        );

        auto s4 = _array4x4[0][3] * (
            _array4x4[1][0] * a32433342 -
            _array4x4[1][1] * a31433341 +
            _array4x4[1][2] * a31423241
        );

        return s1 - s2 + s3 - s4;
    }

    template<typename Type>
    inline void Matrix4x4<Type>::transpose()
    {
        swap(_array4x4[0][1], _array4x4[1][0]);
        swap(_array4x4[0][2], _array4x4[2][0]);
        swap(_array4x4[0][3], _array4x4[3][0]);
        swap(_array4x4[1][2], _array4x4[2][1]);
        swap(_array4x4[1][3], _array4x4[3][1]);
        swap(_array4x4[2][3], _array4x4[3][2]);
    }

    template<typename Type>
    inline void Matrix4x4<Type>::inverse()
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

            _array4x4[0][0] =   tmat[1][1] * a33444334 - tmat[1][2] * a32443442 + tmat[1][3] * a32433342;
            _array4x4[0][1] = -(tmat[1][0] * a33444334 - tmat[1][2] * a31443441 + tmat[1][3] * a31433341);
            _array4x4[0][2] =   tmat[1][0] * a32443442 - tmat[1][1] * a31443441 + tmat[1][3] * a31423241;
            _array4x4[0][3] = -(tmat[1][0] * a32433342 - tmat[1][1] * a31433341 + tmat[1][2] * a31423241);
            
            _array4x4[1][0] = -(tmat[0][1] * a33444334 - tmat[0][2] * a32443442 + tmat[0][3] * a32433342);
            _array4x4[1][1] =   tmat[0][0] * a33444334 - tmat[0][2] * a31443441 + tmat[0][3] * a31433341;
            _array4x4[1][2] = -(tmat[0][0] * a32443442 - tmat[0][1] * a31443441 + tmat[0][3] * a31423241);
            _array4x4[1][3] =   tmat[0][0] * a32433342 - tmat[0][1] * a31433341 + tmat[0][2] * a31423241;
            
            _array4x4[2][0] =   tmat[0][1] * a23442443 - tmat[0][2] * a22444224 + tmat[0][3] * a22432342;
            _array4x4[2][1] = -(tmat[0][0] * a23442443 - tmat[0][2] * a21442441 + tmat[0][3] * a21432341);
            _array4x4[2][2] =   tmat[0][0] * a22444224 - tmat[0][1] * a21442441 + tmat[0][3] * a21422241;
            _array4x4[2][3] = -(tmat[0][0] * a22432342 - tmat[0][1] * a21432341 + tmat[0][2] * a21422241);
            
            _array4x4[3][0] = -(tmat[0][1] * a23342433 - tmat[0][2] * a22342432 + tmat[0][3] * a22332332);
            _array4x4[3][1] =   tmat[0][0] * a23342433 - tmat[0][2] * a21342431 + tmat[0][3] * a21332331;
            _array4x4[3][2] = -(tmat[0][0] * a22342432 - tmat[0][1] * a21342431 + tmat[0][3] * a21322231);
            _array4x4[3][3] =   tmat[0][0] * a22332332 - tmat[0][1] * a21332331 + tmat[0][2] * a21322231;
            
            *this *= (static_cast<Type>(1) / d);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline constexpr Matrix4x4<float>::Matrix4x4() :
        _array16 {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    {}

    inline constexpr Matrix4x4<float>::Matrix4x4(float init_value) :
        _array16 {
            init_value, init_value, init_value, init_value,
            init_value, init_value, init_value, init_value,
            init_value, init_value, init_value, init_value,
            init_value, init_value, init_value, init_value
        }
    {}

    inline constexpr Matrix4x4<float>::Matrix4x4(
        float v11, float v12, float v13, float v14,
        float v21, float v22, float v23, float v24,
        float v31, float v32, float v33, float v34,
        float v41, float v42, float v43, float v44
    ) :
        _array16 {
            v11, v12, v13, v14,
            v21, v22, v23, v24,
            v31, v32, v33, v34,
            v41, v42, v43, v44,
        }
    {}

    inline Matrix4x4<float>::Matrix4x4(const float* ptr_data)
    {
        assert(ptr_data);
        _m256_simd[0] = _mm256_loadu_ps(ptr_data);
        _m256_simd[1] = _mm256_loadu_ps(ptr_data + 8);
    }

    inline Matrix4x4<float>::Matrix4x4(const __m256& s1, const __m256& s2)
    {
        _m256_simd[0] = s1;
        _m256_simd[1] = s2;
    }

    inline bool Matrix4x4<float>::operator == (const Matrix4x4<float>& mat) const
    {
        auto r1 = _mm_cmpeq_ps(_m128_simd[0], mat._m128_simd[0]);
        auto r2 = _mm_cmpeq_ps(_m128_simd[1], mat._m128_simd[1]);
        auto r3 = _mm_cmpeq_ps(_m128_simd[2], mat._m128_simd[2]);
        auto r4 = _mm_cmpeq_ps(_m128_simd[3], mat._m128_simd[3]);
        return _mm_movemask_ps(_mm_and_ps(_mm_and_ps(r1, r2), _mm_and_ps(r3, r4))) == 15;
    }

    inline bool Matrix4x4<float>::operator != (const Matrix4x4<float>& mat) const
    {
        return !(*this == mat);
    }

    inline Matrix4x4<float> Matrix4x4<float>::operator + (const Matrix4x4<float>& mat) const
    {
        return {
            _mm256_add_ps(_m256_simd[0], mat._m256_simd[0]),
            _mm256_add_ps(_m256_simd[1], mat._m256_simd[1])
        };
    }

    inline Matrix4x4<float> Matrix4x4<float>::operator - (const Matrix4x4<float>& mat) const
    {
        return {
            _mm256_sub_ps(_m256_simd[0], mat._m256_simd[0]),
            _mm256_sub_ps(_m256_simd[1], mat._m256_simd[1])
        };
    }

    inline Matrix4x4<float> Matrix4x4<float>::operator * (const Matrix4x4<float>& mat) const
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

    inline Matrix4x4<float> Matrix4x4<float>::operator * (float scal) const
    {
        auto vscal = _mm256_set1_ps(scal);

        return {
            _mm256_mul_ps(_m256_simd[0], vscal),
            _mm256_mul_ps(_m256_simd[1], vscal)
        };
    }

    inline Vec4<float> Matrix4x4<float>::operator * (const Vec4<float>& v) const
    {
        Vec4<float> res;

        res.xyzw_simd = _mm_mul_ps(
            _mm_setr_ps(
                _array4x4[0][0], 
                _array4x4[1][0], 
                _array4x4[2][0], 
                _array4x4[3][0]
            ), 
            _mm_set1_ps(v.xyzw_simd[0])
        );


        res.xyzw_simd = _mm_add_ps(
            _mm_mul_ps(
                _mm_setr_ps(
                    _array4x4[0][1], 
                    _array4x4[1][1], 
                    _array4x4[2][1], 
                    _array4x4[3][1]
                ), 
                _mm_set1_ps(v.xyzw_simd[1])
            ), 
            res.xyzw_simd
        );


        res.xyzw_simd = _mm_add_ps(
            _mm_mul_ps(
                _mm_setr_ps(
                    _array4x4[0][2], 
                    _array4x4[1][2], 
                    _array4x4[2][2], 
                    _array4x4[3][2]
                ), 
                _mm_set1_ps(v.xyzw_simd[2])
            ), 
            res.xyzw_simd
        );

        res.xyzw_simd = _mm_add_ps(
            _mm_mul_ps(
                _mm_setr_ps(
                    _array4x4[0][3], 
                    _array4x4[1][3], 
                    _array4x4[2][3], 
                    _array4x4[3][3]
                ), 
                _mm_set1_ps(v.xyzw_simd[3])
            ), 
            res.xyzw_simd
        );
        
        return res; 
    }

    inline Matrix4x4<float>& Matrix4x4<float>::operator += (const Matrix4x4<float>& mat) 
    {
        _m256_simd[0] = _mm256_add_ps(_m256_simd[0], mat._m256_simd[0]);
        _m256_simd[1] = _mm256_add_ps(_m256_simd[1], mat._m256_simd[1]);
        return *this;
    }

    inline Matrix4x4<float>& Matrix4x4<float>::operator -= (const Matrix4x4<float>& mat) 
    {
        _m256_simd[0] = _mm256_sub_ps(_m256_simd[0], mat._m256_simd[0]);
        _m256_simd[1] = _mm256_sub_ps(_m256_simd[1], mat._m256_simd[1]);
        return *this;
    }

    inline Matrix4x4<float>& Matrix4x4<float>::operator *= (const Matrix4x4<float>& mat) 
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
        
        _array4x4[0][0] = lc1122[0] + lc1122[1] + lc1122[2] + lc1122[3];
        _array4x4[0][1] = lc2112[4] + lc2112[5] + lc2112[6] + lc2112[7];
        _array4x4[0][2] = lc1324[0] + lc1324[1] + lc1324[2] + lc1324[3];
        _array4x4[0][3] = lc2314[4] + lc2314[5] + lc2314[6] + lc2314[7];
        _array4x4[1][0] = lc2112[0] + lc2112[1] + lc2112[2] + lc2112[3];
        _array4x4[1][1] = lc1122[4] + lc1122[5] + lc1122[6] + lc1122[7];
        _array4x4[1][2] = lc2314[0] + lc2314[1] + lc2314[2] + lc2314[3];
        _array4x4[1][3] = lc1324[4] + lc1324[5] + lc1324[6] + lc1324[7];
        _array4x4[2][0] = lc3142[0] + lc3142[1] + lc3142[2] + lc3142[3];
        _array4x4[2][1] = lc4132[4] + lc4132[5] + lc4132[6] + lc4132[7];
        _array4x4[2][2] = lc3344[0] + lc3344[1] + lc3344[2] + lc3344[3];
        _array4x4[2][3] = lc4334[4] + lc4334[5] + lc4334[6] + lc4334[7];
        _array4x4[3][0] = lc4132[0] + lc4132[1] + lc4132[2] + lc4132[3];
        _array4x4[3][1] = lc3142[4] + lc3142[5] + lc3142[6] + lc3142[7];
        _array4x4[3][2] = lc4334[0] + lc4334[1] + lc4334[2] + lc4334[3];
        _array4x4[3][3] = lc3344[4] + lc3344[5] + lc3344[6] + lc3344[7];
        
        return *this;
    }

    inline Matrix4x4<float>& Matrix4x4<float>::operator *= (float scal)
    {
        auto vscal = _mm256_set1_ps(scal);

        _m256_simd[0] = _mm256_mul_ps(_m256_simd[0], vscal);
        _m256_simd[1] = _mm256_mul_ps(_m256_simd[1], vscal);
        return *this;
    }

    inline float* Matrix4x4<float>::operator [] (size_t i) noexcept
    {
        return _array4x4[i];
    }

    inline const float* Matrix4x4<float>::operator [] (size_t i) const noexcept
    {
        return _array4x4[i];
    }

    inline float& Matrix4x4<float>::at(size_t i, size_t j) 
    {
        assert(i < 4 && j < 4);
        return _array4x4[i][j];
    }

    inline float Matrix4x4<float>::at(size_t i, size_t j) const
    {
        assert(i < 4 && j < 4);
        return _array4x4[i][j];
    }

    inline float Matrix4x4<float>::det() const
    {
        float a33444334 = _array4x4[2][2] * _array4x4[3][3] - _array4x4[3][2] * _array4x4[2][3];
        float a32443442 = _array4x4[2][1] * _array4x4[3][3] - _array4x4[2][3] * _array4x4[3][1];
        float a32433342 = _array4x4[2][1] * _array4x4[3][2] - _array4x4[2][2] * _array4x4[3][1];
        float a31443441 = _array4x4[2][0] * _array4x4[3][3] - _array4x4[2][3] * _array4x4[3][0];
        float a31433341 = _array4x4[2][0] * _array4x4[3][2] - _array4x4[2][2] * _array4x4[3][0];
        float a31423241 = _array4x4[2][0] * _array4x4[3][1] - _array4x4[2][1] * _array4x4[3][0];

        float s1 = _array4x4[0][0] * (
            _array4x4[1][1] * a33444334 -
            _array4x4[1][2] * a32443442 +
            _array4x4[1][3] * a32433342
        );

        float s2 = _array4x4[0][1] * (
            _array4x4[1][0] * a33444334 -
            _array4x4[1][2] * a31443441 +
            _array4x4[1][3] * a31433341
        );

        float s3 = _array4x4[0][2] * (
            _array4x4[1][0] * a32443442 -
            _array4x4[1][1] * a31443441 +
            _array4x4[1][3] * a31423241
        );

        float s4 = _array4x4[0][3] * (
            _array4x4[1][0] * a32433342 -
            _array4x4[1][1] * a31433341 +
            _array4x4[1][2] * a31423241
        );

        return s1 - s2 + s3 - s4;
    }

    inline void Matrix4x4<float>::transpose()
    {
        auto xmm0 = _mm_unpacklo_ps(_m128_simd[0], _m128_simd[1]);
        auto xmm1 = _mm_unpacklo_ps(_m128_simd[2], _m128_simd[3]);
        auto xmm2 = _mm_unpackhi_ps(_m128_simd[0], _m128_simd[1]);
        auto xmm3 = _mm_unpackhi_ps(_m128_simd[2], _m128_simd[3]);

        _m128_simd[0] = _mm_movelh_ps(xmm0, xmm1);
        _m128_simd[1] = _mm_movehl_ps(xmm1, xmm0);
        _m128_simd[2] = _mm_movelh_ps(xmm2, xmm3);
        _m128_simd[3] = _mm_movehl_ps(xmm3, xmm2);
    }

    inline void Matrix4x4<float>::inverse()
    {
        auto d = det();

        if (d != static_cast<float>(0u)) {
            __m128 s1, s2, s3, s4;

            auto a = _mm_movelh_ps(_m128_simd[0], _m128_simd[1]);
            auto b = _mm_movehl_ps(_m128_simd[1], _m128_simd[0]);
            auto c = _mm_movelh_ps(_m128_simd[2], _m128_simd[3]);
            auto d = _mm_movehl_ps(_m128_simd[3], _m128_simd[2]);

            float det_a = a[0] * a[3] - a[2] * a[1];
            float det_d = d[0] * d[3] - d[2] * d[1];

            assert(det_a != static_cast<float>(0u) && det_d != static_cast<float>(0u));

            auto adj_a = _mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));
            auto adj_d = _mm_mul_ps(_mm_shuffle_ps(d, d, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));

            auto inv_a = _mm_mul_ps(adj_a, _mm_set1_ps(1.0f / det_a));
            auto inv_d = _mm_mul_ps(adj_d, _mm_set1_ps(1.0f / det_d));

            s1 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(inv_d, inv_d, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(inv_d, inv_d, _MM_SHUFFLE(3, 2, 3, 2));

            auto b_mul_inv_d = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));
            
            s1 = _mm_shuffle_ps(c, c, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(inv_a, inv_a, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(inv_a, inv_a, _MM_SHUFFLE(3, 2, 3, 2));

            auto c_mul_inv_a = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            s1 = _mm_shuffle_ps(b_mul_inv_d, b_mul_inv_d, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(b_mul_inv_d, b_mul_inv_d, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(c, c, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 2, 3, 2));

            auto b_mul_inv_d_mul_c = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            s1 = _mm_shuffle_ps(c_mul_inv_a, c_mul_inv_a, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(c_mul_inv_a, c_mul_inv_a, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 2, 3, 2));

            auto c_mul_inv_a_mul_b = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            auto sm1 = _mm_sub_ps(a, b_mul_inv_d_mul_c);
            auto sm4 = _mm_sub_ps(d, c_mul_inv_a_mul_b);

            float det_sm1 = sm1[0] * sm1[3] - sm1[2] * sm1[1];
            float det_sm4 = sm4[0] * sm4[3] - sm4[2] * sm4[1];

            assert(det_sm1 != static_cast<float>(0u) && det_sm4 != static_cast<float>(0u));

            auto adj_sm1 = _mm_mul_ps(_mm_shuffle_ps(sm1, sm1, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));
            auto adj_sm4 = _mm_mul_ps(_mm_shuffle_ps(sm4, sm4, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));

            auto x = _mm_mul_ps(adj_sm1, _mm_set1_ps(1.0f / det_sm1));
            auto w = _mm_mul_ps(adj_sm4, _mm_set1_ps(1.0f / det_sm4));

            auto sm2 = _mm_mul_ps(x, _mm_set1_ps(-1.0f));
            auto sm3 = _mm_mul_ps(w, _mm_set1_ps(-1.0f));

            s1 = _mm_shuffle_ps(sm2, sm2, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(sm2, sm2, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(b_mul_inv_d, b_mul_inv_d, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(b_mul_inv_d, b_mul_inv_d, _MM_SHUFFLE(3, 2, 3, 2));

            auto y = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            s1 = _mm_shuffle_ps(sm3, sm3, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(sm3, sm3, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(c_mul_inv_a, c_mul_inv_a, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(c_mul_inv_a, c_mul_inv_a, _MM_SHUFFLE(3, 2, 3, 2));

            auto z = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            _m128_simd[0] = _mm_movelh_ps(x, y);
            _m128_simd[1] = _mm_movehl_ps(y, x);
            _m128_simd[2] = _mm_movelh_ps(z, w);
            _m128_simd[3] = _mm_movehl_ps(w, z);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename Type>
    inline Matrix4x4<Type> transpose(const Matrix4x4<Type>& mat)
    {
        return {
            mat[0][0], mat[1][0], mat[2][0], mat[3][0],
            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
            mat[0][3], mat[1][3], mat[2][3], mat[3][3]
        };
    }

    template<typename Type>
    inline Matrix4x4<Type> inverse(const Matrix4x4<Type>& mat)
    {
        Matrix4x4<Type> res (mat);
        res.inverse();
        return res;
    }

    template<typename Type>
    inline ostream& operator << (ostream& print, const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 4; i++) {
            print << mat[i][0] << ' ' << mat[i][1] << ' ' << mat[i][2] << ' ' << mat[i][3] << endl;
        }
        
        return print;
    }
}
