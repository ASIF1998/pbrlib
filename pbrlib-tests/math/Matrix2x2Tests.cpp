//
//  Matrix2x2Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 13/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include <pbrlib/math/matrix2x2.hpp>

using namespace pbrlib::math;

TEST(MathMatrix2x2, Constructor)
{
    constexpr int r1[4] {
        1, 0,
        0, 1
    };

    constexpr int r2[4] {
        23, 0,
        0, 23
    };

    constexpr int r3[4] {
        1, 2, 
        3, 23
    };

    constexpr Matrix2x2<int>    m1;
    constexpr Matrix2x2<int>    m2(23);
    constexpr Matrix2x2<int>    m3(1, 2, 3, 23);
    Matrix2x2<int>              m4(r3);

    pbrlib::testing::utils::equality(m1, r1);
    pbrlib::testing::utils::equality(m2, r2);
    pbrlib::testing::utils::equality(m3, r3);
    pbrlib::testing::utils::equality(m4, r3);
}

TEST(MathMatrix2x2, ConstructorTypeFloat)
{
#if (defined(__SSE__) || defined(__AVX2__))
    union
    {
        __m128 simd;
        float v[4];
    } r4;

    r4.simd = _mm_setr_ps(1.2f, 2.1f, 5.34f, 323.4f);
#endif

    constexpr float r1[4] = {
        1.0f, 0.0f, 
        0.0f, 1.0f
    };

    constexpr float r2[4] = {
        2.3f, 0.0f,
        0.0f, 2.3f
    };

    constexpr float r3[4] = {
        1.20f, 2.100f, 
        5.34f, 323.4f
    };
    
    constexpr Matrix2x2<float> m1;
    constexpr Matrix2x2<float> m2(2.3f);
    constexpr Matrix2x2<float> m3(1.2f, 2.1f, 5.34f, 323.4f);

#if (defined(__SSE__) || defined(__AVX2__))
    Matrix2x2<float> m4(r4.simd);
#endif

    Matrix2x2<float> m5(r3);

    pbrlib::testing::utils::equality(m1, r1);
    pbrlib::testing::utils::equality(m2, r2);
    pbrlib::testing::utils::equality(m3, r3);

#if (defined(__SSE__) || defined(__AVX2__))
    pbrlib::testing::utils::equality(m4, r4.simd);
#endif

    pbrlib::testing::utils::equality(m5, r3);
}

TEST(MathMatrix2x2, EqualAndNotEqual)
{
    constexpr Matrix2x2<short>  m1 (1, 2, 3, 4);
    Matrix2x2<short>            m2 (1, 2, 3, 4);

    pbrlib::testing::utils::thisTrue(m1 == m2);

    m2.at(0, 1) = 6;

    pbrlib::testing::utils::thisTrue(m1 != m2);
}

TEST(MathMatrix2x2, EqualAndNotEqualTypeFloat)
{
    constexpr Matrix2x2<float>  m1 (1.12f, 12.2f, 45.345f, 4.545f);
    Matrix2x2<float>            m2 (1.12f, 12.2f, 45.345f, 4.545f);

    pbrlib::testing::utils::thisTrue(m1 == m2);

    m2.at(0, 1) = 0.43f;

    pbrlib::testing::utils::thisTrue(m1 != m2);
}

TEST(MathMatrix2x2, AdditionAndSubtraction)
{
    constexpr Matrix2x2<int> m1(2);
    constexpr Matrix2x2<int> m2(3);

    Matrix2x2<int> res = m1 + m2;

    pbrlib::testing::utils::equality(Matrix2x2<int>(5), res);

    res = m1 - m2;

    pbrlib::testing::utils::equality(Matrix2x2<int>(-1), res);

    res += m2;

    pbrlib::testing::utils::equality(res, m1);

    res += m2;
    res -= m2;

    pbrlib::testing::utils::equality(res, m1);
}

TEST(MathMatrix2x2, AdditionAndSubtractionTypeFloat)
{
    constexpr Matrix2x2<float> m1(2.5f);
    constexpr Matrix2x2<float> m2(3.0f);

    Matrix2x2<float> res = m1 + m2;

    pbrlib::testing::utils::equality(Matrix2x2<float>(5.5f), res);

    res = m1 - m2;

    pbrlib::testing::utils::equality(Matrix2x2<float>(-0.5f), res);

    res += m2;

    pbrlib::testing::utils::equality(res, m1);

    res += m2;
    res -= m2;

    pbrlib::testing::utils::equality(res, m1);
}

TEST(MathMatrix2x2, ScalarMultiplication)
{
    constexpr Matrix2x2<int>    m   (32);
    constexpr int               s   (4);

    Matrix2x2<int> res (m * s);

    pbrlib::testing::utils::equality(Matrix2x2<int>(128), res);
    
    res *= s;

    pbrlib::testing::utils::equality(Matrix2x2<int>(512), res);
}

TEST(MathMatrix2x2, ScalarMultiplicationTypeFloat)
{
    constexpr Matrix2x2<float>  m   (16.5f);
    constexpr float             s   (4.5f);

    Matrix2x2<float> res = m * s;

    pbrlib::testing::utils::equality(Matrix2x2<float>(74.25f), res);

    res *= (1.0f / s);

    pbrlib::testing::utils::equality(Matrix2x2<float>(16.5f), res);
}

TEST(MathMatrix2x2, MatrixMultiplication)
{
    constexpr Matrix2x2<int> res1 (15, 33, 16, 34);

    constexpr Matrix2x2<int> m1 (3, 2, 6, 7);
    constexpr Matrix2x2<int> m2 (1, 2, 3, 4);

    Matrix2x2<int> res2 = m1 * m2;

    pbrlib::testing::utils::equality(res1, res2);

    res2 = m1;
    res2 *= m2;

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(MathMatrix2x2, MatrixMultiplicationTypeFloat)
{
    constexpr Matrix2x2<float> res1 (
        34.3500023f, 56.4490051f,
        36.6135025f, 4.08415985f
    );

    constexpr Matrix2x2<float> m1 (4.3f, 0.012f, 6.0f, 7.87f);
    constexpr Matrix2x2<float> m2 (1.5f, 4.65f, 12.43f, 0.5f);

    Matrix2x2<float> res2 = m1 * m2;

    pbrlib::testing::utils::equality(res1, res2);

    res2 = m1;
    res2 *= m2;

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(MathMatrix2x2, MatrixAndVectorMultiplication)
{
    constexpr Vec2<int> res1 (13, 14);

    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr Vec2<int>         v (2, 3);

    Vec2<int> res2 = m * v;

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(MathMatrix2x2, MatrixAndVectorMultiplicationTypeFloat)
{
    constexpr Vec2<float> res1 (43.0760002f, 11.8300009f);

    constexpr Matrix2x2<float>  m (1.5f, 4.65f, 12.43f, 0.5f);
    constexpr Vec2<float>       v (2.2f, 3.2f);

    Vec2<float> res2 = m * v;

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(MathMatrix2x2, AccessToElement)
{
    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr int               r[4] {
        2, 1, 3, 4
    };

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            pbrlib::testing::utils::equality(r[i * 2 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            pbrlib::testing::utils::equality(r[i * 2 + j], m.at(i, j));
        }
    }
}

TEST(MathMatrix2x2, AccessToElementTypeFloat)
{
    constexpr Matrix2x2<float>    m (0.0065f, 1.23f, 23.3f, 23.454f);
    constexpr float               r[4] {
        0.0065f, 1.23f, 23.3f, 23.454f
    };

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            pbrlib::testing::utils::equality(r[i * 2 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            pbrlib::testing::utils::equality(r[i * 2 + j], m.at(i, j));
        }
    }
}

TEST(MathMatrix2x2, Determinant)
{
    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr int               r (5);

    pbrlib::testing::utils::equality(r, m.det());
}

TEST(MathMatrix2x2, DeterminantTypeFloat)
{
    constexpr Matrix2x2<float>  m (0.0065f, 1.23f, 23.3f, 23.454f);
    constexpr float             r (-28.506549f);

    pbrlib::testing::utils::equality(r, m.det());
}

TEST(MathMatrix2x2, Transpose)
{
    Matrix2x2<int>  m (2, 1, 3, 4);

    constexpr Matrix2x2<int>    res1 (2, 3, 1, 4);
    Matrix2x2<int>              res2 (transpose(m));

    pbrlib::testing::utils::equality(res1, res2);

    m.transpose();

    pbrlib::testing::utils::equality(res1, m);
}

TEST(MathMatrix2x2, TransposeTypeFloat)
{
    Matrix2x2<float> m (2.2f, 1.2f, 2.3f, 4.4f);

    constexpr Matrix2x2<float>  res1 (2.2f, 2.3f, 1.2f, 4.4f);
    Matrix2x2<float>            res2 (transpose(m));

    pbrlib::testing::utils::equality(res1, res2);

    m.transpose();

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(MathMatrix2x2, Inverse)
{
    constexpr Matrix2x2<int> res1 (1, -2, 0, 1);

    Matrix2x2<int>  m (1, 2, 0, 1);

    Matrix2x2<int> res2 = inverse(m);

    pbrlib::testing::utils::equality(res1, res2);

    m.inverse();

    pbrlib::testing::utils::equality(res1, m);
}

TEST(MathMatrix2x2, InverseTypeFloat)
{
    constexpr Matrix2x2<float> res1 (-2.0f, 1.0f, 1.5f, -0.5f);

    Matrix2x2<float>  m (1.0f, 2.0f, 3.0f, 4.0f);

    Matrix2x2<float> res2 = inverse(m);

    pbrlib::testing::utils::equality(res1, res2);

    m.inverse();

    pbrlib::testing::utils::equality(res1, res2);
}