//
//  matrix4x4.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 22/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <memory>
#include <cassert>

namespace pbrlib::math
{
    template<typename Type>
    inline constexpr Matrix4x4<Type>::Matrix4x4() :
        _array16 {
            static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0), static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1), static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(1)
        }
    {}

    template<typename Type>
    inline constexpr Matrix4x4<Type>::Matrix4x4(Type init_value) :
        _array16 {
            init_value, static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0),
            static_cast<Type>(0), init_value, static_cast<Type>(0), static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(0), init_value, static_cast<Type>(0),
            static_cast<Type>(0), static_cast<Type>(0), static_cast<Type>(0), init_value
        }
    {}

    template<typename Type>
    inline constexpr Matrix4x4<Type>::Matrix4x4(
        Type x0, Type y0, Type z0, Type w0,
        Type x1, Type y1, Type z1, Type w1,
        Type x2, Type y2, Type z2, Type w2,
        Type x3, Type y3, Type z3, Type w3
    ) :
        _array16 {
            x0, y0, z0, w0,
            x1, y1, z1, w1,
            x2, y2, z2, w2,
            x3, y3, z3, w3
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

        for (size_t i{0}; i < 4; ++i) {
            res._vec_array[i] = _vec_array[i] + mat._vec_array[i]; 
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type> Matrix4x4<Type>::operator - (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 4; ++i) {
            res._vec_array[i] = _vec_array[i] - mat._vec_array[i]; 
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type> Matrix4x4<Type>::operator * (const Matrix4x4<Type>& mat) const
    {
        Matrix4x4<Type> res (static_cast<Type>(0));

        for (size_t i{0}; i < 4; i++) {
            for (size_t k{0}; k < 4; k++) {
                auto v = mat._array4x4[i][k];
                for (size_t j{0}; j < 4; j++) {
                    res._array4x4[i][j] += v * _array4x4[k][j];
                }
            }
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type> Matrix4x4<Type>::operator * (Type scal) const
    {
        Matrix4x4<Type> res;

        for (size_t i{0}; i < 4; i++) {
            res._vec_array[i] = _vec_array[i] * scal;
        }

        return res;
    }

    template<typename Type>
    inline Vec4<Type> Matrix4x4<Type>::operator * (const Vec4<Type>& v) const
    {
        Vec4<Type> res;

        for (size_t i{0}; i < 4; i++) {
            res.x += _array4x4[i][0] * v[i];
            res.y += _array4x4[i][1] * v[i];
            res.z += _array4x4[i][2] * v[i];
            res.w += _array4x4[i][3] * v[i];
        }

        return res;
    }

    template<typename Type>
    inline Matrix4x4<Type>& Matrix4x4<Type>::operator += (const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 4; ++i) {
            _vec_array[i] += mat._vec_array[i];
        }

        return *this;
    }

    template<typename Type>
    inline Matrix4x4<Type>& Matrix4x4<Type>::operator -= (const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 4; ++i) {
            _vec_array[i] -= mat._vec_array[i];
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
        for (size_t i{0}; i < 4; i++) {
            _vec_array[i] *= scal;
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

            for (size_t i{0}; i < 16; i++) {
                _array16[i] /= d;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(__AVX2__)
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
            init_value, 0.0f, 0.0f, 0.0f,
            0.0f, init_value, 0.0f, 0.0f,
            0.0f, 0.0f, init_value, 0.0f,
            0.0f, 0.0f, 0.0f, init_value
        }
    {}

    inline constexpr Matrix4x4<float>::Matrix4x4(
        float x0, float y0, float z0, float w0,
        float x1, float y1, float z1, float w1,
        float x2, float y2, float z2, float w2,
        float x3, float y3, float z3, float w3
    ) :
        _array16 {
            x0, y0, z0, w0,
            x1, y1, z1, w1,
            x2, y2, z2, w2,
            x3, y3, z3, w3
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
        Matrix4x4<float> res;

        __m128 b00_vec = _mm_set1_ps(mat._array4x4[0][0]);
        __m128 b01_vec = _mm_set1_ps(mat._array4x4[0][1]);
        __m128 b02_vec = _mm_set1_ps(mat._array4x4[0][2]);
        __m128 b03_vec = _mm_set1_ps(mat._array4x4[0][3]);

        __m128 b10_vec = _mm_set1_ps(mat._array4x4[1][0]);
        __m128 b11_vec = _mm_set1_ps(mat._array4x4[1][1]);
        __m128 b12_vec = _mm_set1_ps(mat._array4x4[1][2]);
        __m128 b13_vec = _mm_set1_ps(mat._array4x4[1][3]);

        __m128 b20_vec = _mm_set1_ps(mat._array4x4[2][0]);
        __m128 b21_vec = _mm_set1_ps(mat._array4x4[2][1]);
        __m128 b22_vec = _mm_set1_ps(mat._array4x4[2][2]);
        __m128 b23_vec = _mm_set1_ps(mat._array4x4[2][3]);

        __m128 b30_vec = _mm_set1_ps(mat._array4x4[3][0]);
        __m128 b31_vec = _mm_set1_ps(mat._array4x4[3][1]);
        __m128 b32_vec = _mm_set1_ps(mat._array4x4[3][2]);
        __m128 b33_vec = _mm_set1_ps(mat._array4x4[3][3]);

        res._m128_simd[0] = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_m128_simd[0], b00_vec), _mm_mul_ps(_m128_simd[1], b01_vec)), _mm_add_ps(_mm_mul_ps(_m128_simd[2], b02_vec), _mm_mul_ps(_m128_simd[3], b03_vec)));
        res._m128_simd[1] = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_m128_simd[0], b10_vec), _mm_mul_ps(_m128_simd[1], b11_vec)), _mm_add_ps(_mm_mul_ps(_m128_simd[2], b12_vec), _mm_mul_ps(_m128_simd[3], b13_vec)));
        res._m128_simd[2] = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_m128_simd[0], b20_vec), _mm_mul_ps(_m128_simd[1], b21_vec)), _mm_add_ps(_mm_mul_ps(_m128_simd[2], b22_vec), _mm_mul_ps(_m128_simd[3], b23_vec)));
        res._m128_simd[3] = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_m128_simd[0], b30_vec), _mm_mul_ps(_m128_simd[1], b31_vec)), _mm_add_ps(_mm_mul_ps(_m128_simd[2], b32_vec), _mm_mul_ps(_m128_simd[3], b33_vec)));

        return res;
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
                _array4x4[0][1], 
                _array4x4[0][2], 
                _array4x4[0][3]
            ), 
            _mm_set1_ps(v.xyzw[0])
        );

        res.xyzw_simd = _mm_add_ps(
            _mm_mul_ps(
                _mm_setr_ps(
                    _array4x4[1][0], 
                    _array4x4[1][1], 
                    _array4x4[1][2], 
                    _array4x4[1][3]
                ), 
                _mm_set1_ps(v.xyzw[1])
            ), 
            res.xyzw_simd
        );

        res.xyzw_simd = _mm_add_ps(
            _mm_mul_ps(
                _mm_setr_ps(
                    _array4x4[2][0], 
                    _array4x4[2][1], 
                    _array4x4[2][2], 
                    _array4x4[2][3]
                ), 
                _mm_set1_ps(v.xyzw[2])
            ), 
            res.xyzw_simd
        );

        res.xyzw_simd = _mm_add_ps(
            _mm_mul_ps(
                _mm_setr_ps(
                    _array4x4[3][0], 
                    _array4x4[3][1], 
                    _array4x4[3][2], 
                    _array4x4[3][3]
                ), 
                _mm_set1_ps(v.xyzw[3])
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
        *this = *this * mat;

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

            union
            {
                __m128 simd;
                float vec[4];
            } a, b, c, d, sm1, sm4;

            a.simd = _mm_movelh_ps(_m128_simd[0], _m128_simd[1]);
            b.simd = _mm_movehl_ps(_m128_simd[1], _m128_simd[0]);
            c.simd = _mm_movelh_ps(_m128_simd[2], _m128_simd[3]);
            d.simd = _mm_movehl_ps(_m128_simd[3], _m128_simd[2]);

            float det_a = a.vec[0] * a.vec[3] - a.vec[2] * a.vec[1];
            float det_d = d.vec[0] * d.vec[3] - d.vec[2] * d.vec[1];

            assert(det_a != static_cast<float>(0u) && det_d != static_cast<float>(0u));

            auto adj_a = _mm_mul_ps(_mm_shuffle_ps(a.simd, a.simd, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));
            auto adj_d = _mm_mul_ps(_mm_shuffle_ps(d.simd, d.simd, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));

            auto inv_a = _mm_mul_ps(adj_a, _mm_set1_ps(1.0f / det_a));
            auto inv_d = _mm_mul_ps(adj_d, _mm_set1_ps(1.0f / det_d));

            s1 = _mm_shuffle_ps(b.simd, b.simd, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(b.simd, b.simd, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(inv_d, inv_d, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(inv_d, inv_d, _MM_SHUFFLE(3, 2, 3, 2));

            auto b_mul_inv_d = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            s1 = _mm_shuffle_ps(c.simd, c.simd, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(c.simd, c.simd, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(inv_a, inv_a, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(inv_a, inv_a, _MM_SHUFFLE(3, 2, 3, 2));

            auto c_mul_inv_a = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            s1 = _mm_shuffle_ps(b_mul_inv_d, b_mul_inv_d, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(b_mul_inv_d, b_mul_inv_d, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(c.simd, c.simd, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(c.simd, c.simd, _MM_SHUFFLE(3, 2, 3, 2));

            auto b_mul_inv_d_mul_c = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            s1 = _mm_shuffle_ps(c_mul_inv_a, c_mul_inv_a, _MM_SHUFFLE(2, 2, 0, 0));
            s2 = _mm_shuffle_ps(c_mul_inv_a, c_mul_inv_a, _MM_SHUFFLE(3, 3, 1, 1));
            s3 = _mm_shuffle_ps(b.simd, b.simd, _MM_SHUFFLE(1, 0, 1, 0));
            s4 = _mm_shuffle_ps(b.simd, b.simd, _MM_SHUFFLE(3, 2, 3, 2));

            auto c_mul_inv_a_mul_b = _mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, s4));

            sm1.simd = _mm_sub_ps(a.simd, b_mul_inv_d_mul_c);
            sm4.simd = _mm_sub_ps(d.simd, c_mul_inv_a_mul_b);

            float det_sm1 = sm1.vec[0] * sm1.vec[3] - sm1.vec[2] * sm1.vec[1];
            float det_sm4 = sm4.vec[0] * sm4.vec[3] - sm4.vec[2] * sm4.vec[1];

            assert(det_sm1 != static_cast<float>(0u) && det_sm4 != static_cast<float>(0u));

            auto adj_sm1 = _mm_mul_ps(_mm_shuffle_ps(sm1.simd, sm1.simd, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));
            auto adj_sm4 = _mm_mul_ps(_mm_shuffle_ps(sm4.simd, sm4.simd, _MM_SHUFFLE(0, 2, 1, 3)), _mm_setr_ps(1, -1, -1, 1));

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
#endif

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
    inline std::ostream& operator << (std::ostream& print, const Matrix4x4<Type>& mat)
    {
        for (size_t i{0}; i < 4; i++)
            print << mat[i][0] << ' ' << mat[i][1] << ' ' << mat[i][2] << ' ' << mat[i][3] << std::endl;
        
        return print;
    }
}
