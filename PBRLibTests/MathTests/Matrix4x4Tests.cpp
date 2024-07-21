//
//  Matrix4x4Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 13/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/vec4.hpp>

using namespace pbrlib::math;

TEST(MathMatrix4x4, Constructor)
{
    constexpr Matrix4x4<int> m1;
    constexpr Matrix4x4<int> m2 (23);

    constexpr Matrix4x4<int> m3 {
        1, 2, 3, 4,
        4, 3, 2, 1,
        4, 5, 6, 7,
        8, 6, 3, 1
    };

    constexpr int r1[16] {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0, 
        0, 0, 0, 1
    };

    constexpr int r2[16] {
        23, 0, 0, 0,
        0, 23, 0, 0,
        0, 0, 23, 0,
        0, 0, 0, 23
    };

    constexpr int r3[16] {
        1, 2, 3, 4,
        4, 3, 2, 1,
        4, 5, 6, 7,
        8, 6, 3, 1
    };

    pbrlib::testing::utils::equality(m1, r1);
    pbrlib::testing::utils::equality(m2, r2);
    pbrlib::testing::utils::equality(m3, r3);
}

TEST(MathMatrix4x4, ConstructorTypeFloat)
{
    constexpr Matrix4x4<float> m1;
    constexpr Matrix4x4<float> m2 (2.3f);

    constexpr Matrix4x4<float> m3 {
        1.23f, 3.32f, 34.5f, 4.43f,
        4.32f, 23.3f, 4.52f, 3.51f,
        43.0f, 5.03f, 0.06f, 7.23f,
        23.8f, 2.46f, 3.53f, 13.4f
    };

    constexpr float r1[16] {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };

    constexpr float r2[16] {
        2.3f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.3f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.3f, 0.0f,
        0.0f, 0.0f, 0.0f, 2.3f
    };

    constexpr float r3[16] {
        1.23f, 3.32f, 34.5f, 4.43f,
        4.32f, 23.3f, 4.52f, 3.51f,
        43.0f, 5.03f, 0.06f, 7.23f,
        23.8f, 2.46f, 3.53f, 13.4f
    };

    pbrlib::testing::utils::equality(m1, r1);
    pbrlib::testing::utils::equality(m2, r2);
    pbrlib::testing::utils::equality(m3, r3);
}

TEST(MathMatrix4x4, EqualAndNotEqual)
{
    constexpr Matrix4x4<int> m1 {
        1, 2, 3, 4,
        3, 2, 5, 6, 
        1, 9, 5, 8,
        1, 5, 6, 4
    };

    Matrix4x4<int> m2 {
        1, 2, 3, 4,
        3, 2, 5, 6, 
        1, 9, 5, 8,
        1, 5, 6, 4
    };

    pbrlib::testing::utils::thisTrue(m1 == m2);

    m2.at(2, 3) = 23;

    pbrlib::testing::utils::thisTrue(m1 != m2);
}

TEST(MathMatrix4x4, EqualAndNotEqualTypeFloat)
{
    constexpr Matrix4x4<float> m1 {
        1.23f, 3.32f, 34.5f, 4.43f,
        4.32f, 23.3f, 4.52f, 3.51f,
        43.0f, 5.03f, 0.06f, 7.23f,
        23.8f, 2.46f, 3.53f, 13.4f
    };

    Matrix4x4<float> m2 {
        1.23f, 3.32f, 34.5f, 4.43f,
        4.32f, 23.3f, 4.52f, 3.51f,
        43.0f, 5.03f, 0.06f, 7.23f,
        23.8f, 2.46f, 3.53f, 13.4f
    };

    pbrlib::testing::utils::thisTrue(m1 == m2);

    m2.at(2, 3) = 23.23f;

    pbrlib::testing::utils::thisTrue(m1 != m2);
}

TEST(MathMatrix4x4, AdditionAndSubtraction)
{
    constexpr Matrix4x4<short> m1 (23);
    constexpr Matrix4x4<short> m2 (37);

    Matrix4x4<short> res = m1 + m2;

    pbrlib::testing::utils::equality(Matrix4x4<short>(60), res);

    res = m1 - m2;

    pbrlib::testing::utils::equality(Matrix4x4<short>(-14), res);

    res =   m1;
    res +=  m2;

    pbrlib::testing::utils::equality(Matrix4x4<short>(60), res);

    res -= m2;

    pbrlib::testing::utils::equality(Matrix4x4<short>(23), res);
}

TEST(MathMatrix4x4, AdditionAndSubtractionTypeFloat)
{
    constexpr Matrix4x4<float> m1 (23.0f);
    constexpr Matrix4x4<float> m2 (37.0f);

    Matrix4x4<float> res = m1 + m2;

    pbrlib::testing::utils::equality(Matrix4x4<float>(60.0f), res);

    res = m1 - m2;

    pbrlib::testing::utils::equality(Matrix4x4<float>(-14.0f), res);

    res =   m1;
    res +=  m2;

    pbrlib::testing::utils::equality(Matrix4x4<float>(60.0f), res);

    res -= m2;

    pbrlib::testing::utils::equality(Matrix4x4<float>(23.0f), res);
}

TEST(MathMatrix4x4, ScalarMultiplication)
{
    constexpr Matrix4x4<int>    m (16);
    constexpr int               s (2);

    Matrix4x4<int> res = m * s;

    pbrlib::testing::utils::equality(Matrix4x4<int>(32), res);

    res *= s;

    pbrlib::testing::utils::equality(Matrix4x4<int>(64), res);
}

TEST(MathMatrix4x4, ScalarMultiplicationTypeFloat)
{
    constexpr Matrix4x4<float>    m (16.5f);
    constexpr float               s (2.0f);

    Matrix4x4<float> res = m * s;

    pbrlib::testing::utils::equality(Matrix4x4<float>(33.0f), res);

    res *= s;

    pbrlib::testing::utils::equality(Matrix4x4<float>(66.0f), res);
}

TEST(MathMatrix4x4, MatrixMultiplication)
{
    constexpr Matrix4x4<int> m1 {
        6, 4, 5, 3, 
        5, 3, 2, 5,
        7, 5, 3, 2,
        3, 5, 2, 1
    };

    constexpr Matrix4x4<int> m2 {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    Matrix4x4<int> r1 = m1 * m2;

    constexpr Matrix4x4<int> r2 {
        49,  45, 26, 23,
        101, 77, 57, 50,
        98,  72, 55, 49,
        106, 76, 63, 61
    };

    pbrlib::testing::utils::equality(r1, r2);

    r1 =     m1;
    r1 *=    m2;

    pbrlib::testing::utils::equality(r1, r2);
}

TEST(MathMatrix4x4, MatrixMultiplicationTypeFloat)
{
    constexpr Matrix4x4<float> m1 {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    constexpr Matrix4x4<float> m2 {
        6.0f, 4.00f, 5.0f, 3.3f, 
        5.3f, 0.05f, 2.0f, 5.0f,
        7.0f, 5.00f, 3.0f, 2.0f,
        3.0f, 5.00f, 2.0f, 1.0f
    };

    Matrix4x4<float> r1 = m1 * m2;

    constexpr Matrix4x4<float> r2 {
        74.0999985f, 67.8000030f, 85.1999969f, 52.5999985f,
        50.5499992f, 48.7999992f, 48.2000008f, 35.3499985f,
        61.0000000f, 58.0000000f, 77.0000000f, 53.0000000f,
        45.0000000f, 40.0000000f, 55.0000000f, 33.0000000f
    };

    pbrlib::testing::utils::equality(r1, r2);

    r1 =     m1;
    r1 *=    m2;

    pbrlib::testing::utils::equality(r1, r2);
}

TEST(MathMatrix4x4, MatrixAndVectorMultiplication)
{
    constexpr Matrix4x4<int> m {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    constexpr Vec4<int> v   (2, 1, 3, 4);
    constexpr Vec4<int> r1  (50, 44, 46, 25);

    Vec4<int> r2 = m * v;

    pbrlib::testing::utils::equality(r1, r2);
}

TEST(MathMatrix4x4, MatrixAndVectorMultiplicationTypeFloat)
{
    constexpr Matrix4x4<float> m {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    constexpr Vec4<float> v     (2.2f, 23.14f, 3.34f, 4.04f);
    constexpr Vec4<float> r1    (162.879990f, 134.559998f, 181.639999f, 92.9800034f);

    Vec4<float> r2 = m * v;

    pbrlib::testing::utils::equality(r1, r2);
}

TEST(MathMatrix4x4, AccessToElement)
{
    constexpr Matrix4x4<int> m {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    constexpr int r[16] {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    for (size_t i{0}; i < 4; i++) {
        for (size_t j{0}; j < 4; j++) {
            pbrlib::testing::utils::equality(r[i * 4 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 4; i++) {
        for (size_t j{0}; j < 4; j++) {
            pbrlib::testing::utils::equality(r[i * 4 + j], m.at(i, j));
        }
    }
}

TEST(MathMatrix4x4, AccessToElementTypeFloat)
{
    constexpr Matrix4x4<float> m {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    constexpr float r[16] {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    for (size_t i{0}; i < 4; i++) {
        for (size_t j{0}; j < 4; j++) {
            pbrlib::testing::utils::equality(r[i * 4 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 4; i++) {
        for (size_t j{0}; j < 4; j++) {
            pbrlib::testing::utils::equality(r[i * 4 + j], m.at(i, j));
        }
    }
}

TEST(MathMatrix4x4, Determinant)
{
    constexpr Matrix4x4<int> m {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    constexpr int r = 30;

    pbrlib::testing::utils::equality(r, m.det());
}

TEST(MathMatrix4x4, DeterminantTypeFloat)
{
    constexpr Matrix4x4<float> m {
        1.0f, 2.00f, 3.3f, 4.0f,
        5.0f, 4.00f, 6.0f, 3.0f,
        5.0f, 34.4f, 6.6f, 2.0f,
        7.0f, 6.00f, 4.0f, 2.0f
    };

    constexpr float r = 1614.28f;

    pbrlib::testing::utils::equality(r, m.det());
}

TEST(MathMatrix4x4, Transpose)
{
    Matrix4x4<int> m {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    constexpr Matrix4x4<int> r {
        1, 5, 5, 7, 
        2, 4, 4, 6, 
        3, 6, 6, 4, 
        4, 3, 2, 2
    };

    pbrlib::testing::utils::equality(r, transpose(m));

    m.transpose();

    pbrlib::testing::utils::equality(r, m);
}

TEST(MathMatrix4x4, TransposeTypeFloat)
{
    Matrix4x4<float> m {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    constexpr Matrix4x4<float> r {
        1.0f, 5.0f, 5.0f, 7.0f, 
        2.0f, 4.0f, 4.0f, 6.0f, 
        3.0f, 6.0f, 6.0f, 4.0f, 
        4.0f, 3.0f, 2.0f, 2.0f
    };

    pbrlib::testing::utils::equality(r, transpose(m));

    m.transpose();

    pbrlib::testing::utils::equality(r, m);
}

TEST(MathMatrix4x4, Inverse)
{
    constexpr Matrix4x4<int> m {
        1, 3, 3, 4,
        5, 6, 7, 8,
        8, 7, 6, 5,
        5, 5, 2, 1
    };

    constexpr Matrix4x4<int> r {
        -2, 1, -1, 1, 
        1, -0, 0, 0,
        4, -4, 3, -3,
        -3, 3, -2, 2
    };

    pbrlib::testing::utils::equality(r, inverse(m));
}

TEST(MathMatrix4x4, InverseTypeFloat)
{
    constexpr Matrix4x4<float> m {
        1.0f, 3.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f,
        5.0f, 5.0f, 2.0f, 1.0f
    };

    constexpr Matrix4x4<float> r {
        -2.00000000f, 1.6923077100f, -1.30769229f, 1.000000000f,
        1.000000000f, -0.692307711f, 0.307692289f, 0.000000000f,
        3.999994280f, -4.076917650f, 3.923071620f, -2.99999619f,
        -2.99999619f, 3.1538419700f, -2.84615016f, 1.999997380f
    };

    pbrlib::testing::utils::equality(r, inverse(m));
}