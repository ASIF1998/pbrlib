//
//  Matrix2x2Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 13/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/math/matrix2x2.hpp"

using namespace testing;
using namespace pbrlib::math;

TEST(MathMatrix2x2, Constructor)
{
    __m128 r4 = _mm_setr_ps(1.2f, 2.1f, 5.34f, 323.4f);

    constexpr float r1[4] = {
        1.0f, 0.0f, 
        0.0f, 1.0f
    };

    constexpr float r2[4] = {
        2.3f, 0.0f,
        0.0f, 2.3f
    };

    constexpr float r3[4] = {
        1.2f, 2.1f, 
        5.34f, 323.4f
    };
    
    constexpr Matrix2x2<float>  m1;
    constexpr Matrix2x2<float>  m2(2.3f);
    constexpr Matrix2x2<float>  m3(1.2f, 2.1f, 5.34f, 323.4f);
    Matrix2x2<float>            m4(r4);
    Matrix2x2<float>            m5(r3);

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r1[i * 2 + j], m1.at(i, j));
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r2[i * 2 + j], m2.at(i, j));
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r3[i * 2 + j], m3.at(i, j));
        }
    }
    
    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r3[i * 2 + j], m3.at(i, j));
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r3[i * 2 + j], m5.at(i, j));
        }
    }
}

TEST(MathMatrix2x2, ConstructorTypeFloat)
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

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r1[i * 2 + j], m1.at(i, j));
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r2[i * 2 + j], m2.at(i, j));
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r3[i * 2 + j], m3.at(i, j));
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r3[i * 2 + j], m4.at(i, j));
        }
    }
}

TEST(MathMatrix2x2, EqualAndNotEqual)
{
    constexpr Matrix2x2<short>  m1 (1, 2, 3, 4);
    Matrix2x2<short>            m2 (1, 2, 3, 4);

    EXPECT_TRUE(m1 == m2);

    m2.at(0, 1) = 6;

    EXPECT_TRUE(m1 != m2);
}

TEST(MathMatrix2x2, EqualAndNotEqualTypeFloat)
{
    constexpr Matrix2x2<float>  m1 (1.12f, 12.2f, 45.345f, 4.545f);
    Matrix2x2<float>            m2 (1.12f, 12.2f, 45.345f, 4.545f);

    EXPECT_TRUE(m1 == m2);

    m2.at(0, 1) = .43f;

    EXPECT_TRUE(m1 != m2);
}

TEST(MathMatrix2x2, AdditionAndSubtraction)
{
    constexpr Matrix2x2<int> m1(2);
    constexpr Matrix2x2<int> m2(3);

    Matrix2x2<int> res = m1 + m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(5, res[i][j]);
            } else {
                EXPECT_EQ(0, res[i][j]);
            }
        }
    }

    res = m1 - m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(-1, res[i][j]);
            } else {
                EXPECT_NE(-1, res[i][j]);
            }
        }
    }

    res += m2;

    EXPECT_EQ(res, m1);

    res += m2;
    res -= m2;

    EXPECT_EQ(res, m1);
}

TEST(MathMatrix2x2, AdditionAndSubtractionTypeFloat)
{
    constexpr Matrix2x2<float> m1(2.5f);
    constexpr Matrix2x2<float> m2(3.0f);

    Matrix2x2<float> res = m1 + m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(5.5f, res[i][j]);
            } else {
                EXPECT_NE(5.5f, res[i][j]);
            }
        }
    }

    res = m1 - m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(-0.5f, res[i][j]);
            } else {
                EXPECT_NE(-0.5f, res[i][j]);
            }
        }
    }

    res += m2;

    EXPECT_EQ(res, m1);

    res += m2;
    res -= m2;

    EXPECT_EQ(res, m1);
}

TEST(MathMatrix2x2, ScalarMultiplication)
{
    constexpr Matrix2x2<int>    m   (32);
    constexpr int               s   (4);

    Matrix2x2<int> res (m * s);

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(128, res[i][j]);
            } else {
                EXPECT_NE(128, res[i][j]);
            }
        }
    }
    
    res *= s;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(512, res[i][j]);
            } else {
                EXPECT_NE(512, res[i][j]);
            }
            
        }
    }
}

TEST(MathMatrix2x2, ScalarMultiplicationTypeFloat)
{
    constexpr Matrix2x2<float>  m   (16.5f);
    constexpr float             s   (4.5f);

    Matrix2x2<float> res = m * s;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(74.25f, res[i][j]);
            } else {
                EXPECT_NE(74.25f, res[i][j]);
            }
        }
    }

    res *= (1.0f / s);

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            if (i == j) {
                EXPECT_EQ(16.5f, res[i][j]);
            } else {
                EXPECT_NE(16.5f, res[i][j]);
            }
        }
    }
}

TEST(MathMatrix2x2, MatrixMultiplication)
{
    constexpr int res1[4] {
        15, 16,
        33, 34
    };

    constexpr Matrix2x2<int> m1 (1, 2, 3, 4);
    constexpr Matrix2x2<int> m2 (3, 2, 6, 7);

    Matrix2x2<int> res2 = m1 * m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(res1[i * 2 + j], res2.at(i, j));
        }
    }

    res2 = m1;
    res2 *= m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(res1[i * 2 + j], res2.at(i, j));
        }
    }
}

TEST(MathMatrix2x2, MatrixMultiplicationTypeFloat)
{
    constexpr float res1[4] {
        34.3500023f, 36.6135025f,
        56.4490051f, 4.08415985f
    };

    constexpr Matrix2x2<float> m1 (1.5f, 4.65f, 12.43f, 0.5f);
    constexpr Matrix2x2<float> m2 (4.3f, 0.012f, 6.0f, 7.87f);

    Matrix2x2<float> res2 = m1 * m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(res1[i * 2 + j], res2.at(i, j));
        }
    }

    res2 = m1;
    res2 *= m2;

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(res1[i * 2 + j], res2.at(i, j));
        }
    }
}

TEST(MathMatrix2x2, MatrixAndVectorMultiplication)
{
    constexpr Vec2<int> res1 (7, 18);

    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr Vec2<int>         v (2, 3);

    Vec2<int> res2 = m * v;

    EXPECT_EQ(res1, res2);
}

TEST(MathMatrix2x2, MatrixAndVectorMultiplicationTypeFloat)
{
    constexpr Vec2<float> res1 (18.1800003f, 28.9460011f);

    constexpr Matrix2x2<float>  m (1.5f, 4.65f, 12.43f, 0.5f);
    constexpr Vec2<float>       v (2.2f, 3.2f);

    Vec2<float> res2 = m * v;

    EXPECT_EQ(res1, res2);
}

TEST(MathMatrix2x2, AccessToElement)
{
    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr int               r[4] {
        2, 1, 3, 4
    };

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r[i * 2 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r[i * 2 + j], m.at(i, j));
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
            EXPECT_EQ(r[i * 2 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 2; i++) {
        for (size_t j{0}; j < 2; j++) {
            EXPECT_EQ(r[i * 2 + j], m.at(i, j));
        }
    }
}

TEST(MathMatrix2x2, Determinant)
{
    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr int               r (5);

    EXPECT_EQ(r, m.det());
}

TEST(MathMatrix2x2, DeterminantTypeFloat)
{
    constexpr Matrix2x2<float>  m (0.0065f, 1.23f, 23.3f, 23.454f);
    constexpr float             r (-28.506549f);

    EXPECT_EQ(r, m.det());
}

TEST(MathMatrix2x2, Transpose)
{
    Matrix2x2<int>  m (2, 1, 3, 4);

    constexpr Matrix2x2<int> res1 (2, 3, 1, 4);

    Matrix2x2<int> res2 = transpose(m);

    EXPECT_EQ(res1, res2);
    m.transpose();
    EXPECT_EQ(res1, m);
}

TEST(MathMatrix2x2, TransposeTypeFloat)
{
    Matrix2x2<float> m (2.2f, 1.2f, 2.3f, 4.4f);

    constexpr Matrix2x2<float> res1 (2.2f, 2.3f, 1.2f, 4.4f);

    Matrix2x2<float> res2 = transpose(m);

    EXPECT_EQ(res1, res2);
    m.transpose();
    EXPECT_EQ(res1, m);
}

TEST(MathMatrix2x2, Inverse)
{
    constexpr Matrix2x2<int> res1 (1, -2, 0, 1);

    Matrix2x2<int>  m (1, 2, 0, 1);

    Matrix2x2<int> res2 = inverse(m);

    EXPECT_EQ(res1, res2);
    m.inverse();
    EXPECT_EQ(res1, m);
}

TEST(MathMatrix2x2, InverseTypeFloat)
{
    constexpr Matrix2x2<float> res1 (-2.0f, 1.0f, 1.5f, -0.5f);

    Matrix2x2<float>  m (1.0f, 2.0f, 3.0f, 4.0f);

    Matrix2x2<float> res2 = inverse(m);

    EXPECT_EQ(res1, res2);
    m.inverse();
    EXPECT_EQ(res1, m);
}
