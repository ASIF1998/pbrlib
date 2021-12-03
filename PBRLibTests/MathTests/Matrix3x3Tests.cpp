//
//  Matrix3x3Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 13/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include "../../src/math/matrix3x3.hpp"
#include "../../src/math/vec3.hpp"

using namespace pbrlib::math;

TEST(MathMatrix3x3, Constructor)
{
    constexpr Matrix3x3<int> m1;
    constexpr Matrix3x3<int> m2 (23);

    constexpr Matrix3x3<int> m3 (
        1, 2, 3,
        2, 3, 4,
        3, 4, 5
    );

    constexpr int r1[9] {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    constexpr int r2[9] {
        23, 0, 0,
        0, 23, 0,
        0, 0, 23
    };

    constexpr int r3[9] {
        1, 2, 3,
        2, 3, 4,
        3, 4, 5
    };

    pbrlib::testing::utils::equality(m1, r1);
    pbrlib::testing::utils::equality(m2, r2);
    pbrlib::testing::utils::equality(m3, r3);
}

TEST(MathMatrix3x3, EqualAndNotEqual)
{
    constexpr Matrix3x3<int> m1 (
        1, 2, 3,
        2, 3, 4,
        3, 4, 5
    );

    Matrix3x3<int> m2 (
        1, 2, 3,
        2, 3, 4,
        3, 4, 5
    );

    pbrlib::testing::utils::thisTrue(m1 == m2);

    m2.at(1, 2) = 60;

    pbrlib::testing::utils::thisTrue(m1 != m2);
}

TEST(MathMatrix3x3, AdditionAndSubtraction)
{
    constexpr Matrix3x3<short> m1 (23);
    constexpr Matrix3x3<short> m2 (37);

    Matrix3x3<short> res = m1 + m2;

    pbrlib::testing::utils::equality(Matrix3x3<short>(60), res);

    res = m1 - m2;

    pbrlib::testing::utils::equality(Matrix3x3<short>(-14), res);

    res =   m1;
    res +=  m2;

    pbrlib::testing::utils::equality(Matrix3x3<short>(60), res);

    res -= m2;

    pbrlib::testing::utils::equality(Matrix3x3<short>(23), res);
}

TEST(MathMatrix3x3, ScalarMultiplication)
{
    constexpr Matrix3x3<int>    m (16);
    constexpr int               s (2);

    Matrix3x3<int> res = m * s;

    pbrlib::testing::utils::equality(Matrix3x3<int>(32), res);
    
    res *= s;

    pbrlib::testing::utils::equality(Matrix3x3<int>(64), res);
}

TEST(MathMatrix3x3, MatrixMultiplication)
{
    constexpr Matrix3x3<float> m1 {
        10.125f, 23.3f, 0.250f,
        4.3500f, 3.20f, 0.450f,
        2.5340f, 8.59f, 33.43f
    };

    constexpr Matrix3x3<float> m2 {
        1.200f, 3.400f, 0.0500f,
        4.300f, 34.00f, 4.4500f,
        23.54f, 82.59f, 323.43f
    };

    Matrix3x3<float> r = m1 * m2;

    constexpr Matrix3x3<float> res {
        27.0667000f, 39.269500f, 3.50150013f,
        202.713800f, 247.21550f, 165.138489f,
        1417.18062f, 3591.0337f, 10855.3154f  
    };

    pbrlib::testing::utils::equality(r, res);

    r =     m1;
    r *=    m2;

    pbrlib::testing::utils::equality(r, res);
}

TEST(MathMatrix3x3, MatrixAndVectorMultiplication)
{
    constexpr Matrix3x3<int> m {
        1, 2, 3, 
        6, 4, 3,
        9, 7, 3
    };

    constexpr Vec3<int> v (1, 3, 23);

    Vec3<int> res (226, 175, 81);

    pbrlib::testing::utils::equality(res, m * v);
}   

TEST(MathMatrix3x3, AccessToElement)
{
    constexpr Matrix3x3<int> m {
        1, 2, 3, 
        6, 4, 3,
        9, 7, 3
    };

    constexpr int r[9] {
        1, 2, 3, 
        6, 4, 3,
        9, 7, 3
    };

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            pbrlib::testing::utils::equality(r[i * 3 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            pbrlib::testing::utils::equality(r[i * 3 + j], m.at(i, j));
        }
    }
}

TEST(MathMatrix3x3, Determinant)
{
    constexpr Matrix3x3<float> m {
        1.0f, 2.0f, 3.50f,
        2.2f, 3.0f, 1.20f,
        8.5f, 2.6f, 12.3f
    };
    
    constexpr float r = -69.1700058f;

    pbrlib::testing::utils::equality(r, m.det());
}

TEST(MathMatrix3x3, Transpose)
{
    constexpr Matrix3x3<float> res {
        1.0f, 2.0f, 3.5f,
        2.2f, 3.0f, 1.2f,
        8.5f, 2.6f, 12.3f
    };

    Matrix3x3<float> m {
        1.0f, 2.2f, 8.50f,
        2.0f, 3.0f, 2.60f,
        3.5f, 1.2f, 12.3f
    };

    pbrlib::testing::utils::equality(res, transpose(m));

    m.transpose();

    pbrlib::testing::utils::equality(res, m);
}

TEST(MathMatrix3x3, Inverse)
{
    constexpr Matrix3x3<float> res {
        1.0f, 2.0f, 3.5f,
        2.2f, 3.0f, 1.2f,
        8.5f, 2.6f, 12.3f
    };

    Matrix3x3<float> m {
        -0.488362006f, 0.224085586236808f, 0.117102790226977f,
        0.2437472790f, 0.252276998698858f, -0.09397136420000f,
        0.2859621050f, -0.20818272200000f, 0.020239988300000f
    };

    pbrlib::testing::utils::equality(res, inverse(m));
}
