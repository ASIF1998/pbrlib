//
//  Matrix3x3Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 13/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/math/matrix3x3.hpp"
#include "../../src/math/vec3.hpp"

using namespace testing;
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
        23, 23, 23,
        23, 23, 23,
        23, 23, 23
    };

    constexpr int r3[9] {
        1, 2, 3,
        2, 3, 4,
        3, 4, 5
    };

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(r1[i * 3 + j], m1[i][j]);
        }
    }

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(r2[i * 3 + j], m2[i][j]);
        }
    }

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(r3[i * 3 + j], m3[i][j]);
        }
    }
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

    EXPECT_TRUE(m1 == m2);

    m2.at(1, 2) = 60;

    EXPECT_TRUE(m1 != m2);
}

TEST(MathMatrix3x3, AdditionAndSubtraction)
{
    constexpr Matrix3x3<short> m1 (23);
    constexpr Matrix3x3<short> m2 (37);

    Matrix3x3<short> res = m1 + m2;

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(60, res[i][j]);
        }
    }

    res = m1 - m2;

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(-14, res[i][j]);
        }
    }

    res = m1;

    res += m2;

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(60, res[i][j]);
        }
    }

    res -= m2;

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(23, res[i][j]);
        }
    }
}

TEST(MathMatrix3x3, ScalarMultiplication)
{
    constexpr Matrix3x3<int>    m (16);
    constexpr int               s (2);

    Matrix3x3<int> res = m * s;

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(32, res[i][j]);
        }
    }

    res *= s;

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(64, res[i][j]);
        }
    }
}

TEST(MathMatrix3x3, MatrixMultiplication)
{
    constexpr Matrix3x3<float> m1 {
        1.2f,   3.4f,   0.05f,
        4.3f,   34.0f,  4.45f,
        23.54f, 82.59f, 323.43f
    };

    constexpr Matrix3x3<float> m2 {
        10.125f,    23.3f,  0.25f,
        4.35f,      3.2f,   0.45f,
        2.534f,     8.59f,  33.43f
    };

    Matrix3x3<float> r = m1 * m2;

    constexpr Matrix3x3<float> res {
        27.0667f,       39.2695f,   3.50150013f,
        202.7138f,      247.2155f,  165.138489f,
        1417.18062f,    3591.0337f, 10855.3154f  
    };

    EXPECT_EQ(r, res);

    r =     m1;
    r *=    m2;

    EXPECT_EQ(r, res);
}

TEST(MathMatrix3x3, MatrixAndVectorMultiplication)
{
    constexpr Matrix3x3<int> m {
        1, 2, 3, 
        6, 4, 3,
        9, 7, 3
    };

    constexpr Vec3<int> v (1, 3, 23);

    Vec3<int> res (76, 87, 99);

    EXPECT_EQ(res, m * v);
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
            EXPECT_EQ(r[i * 3 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 3; i++) {
        for (size_t j{0}; j < 3; j++) {
            EXPECT_EQ(r[i * 3 + j], m.at(i, j));
        }
    }
}

TEST(MathMatrix3x3, Determinant)
{
    constexpr Matrix3x3<float> m {
        1.0f, 2.0f, 3.5f,
        2.2f, 3.0f, 1.2f,
        8.5f, 2.6f, 12.3f
    };
    
    constexpr float r = -69.1700058f;

    EXPECT_EQ(r, m.det());
}

TEST(MathMatrix3x3, Transpose)
{
    constexpr Matrix3x3<float> m1 {
        1.0f, 2.0f, 3.5f,
        2.2f, 3.0f, 1.2f,
        8.5f, 2.6f, 12.3f
    };

    Matrix3x3<float> m2 {
        1.0f, 2.2f, 8.5f,
        2.0f, 3.0f, 2.6f,
        3.5f, 1.2f, 12.3f
    };

    EXPECT_EQ(m1, transpose(m2));
    m2.transpose();
    EXPECT_EQ(m1, m2);
}

TEST(MathMatrix3x3, Inverse)
{
    constexpr Matrix3x3<float> m1 {
        1.0f, 2.0f, 3.5f,
        2.2f, 3.0f, 1.2f,
        8.5f, 2.6f, 12.3f
    };

    Matrix3x3<float> m2 {
        -0.488362006f,  0.224085586236808f, 0.117102790226977f,
        0.243747279f,   0.252276998698858f, -0.0939713642,
        0.285962105f,   -0.208182722f,      0.0202399883f
    };
    
    EXPECT_EQ(m2, inverse(m1));
}
