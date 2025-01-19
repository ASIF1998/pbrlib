#include "../utils/assertions.hpp"

#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/vec4.hpp>

using namespace pbrlib::math;

TEST(Mat4Tests, Constructor)
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

    pbrlib::testing::equality(m1, r1);
    pbrlib::testing::equality(m2, r2);
    pbrlib::testing::equality(m3, r3);
}

TEST(Mat4Tests, ConstructorTypeFloat)
{
    constexpr mat4 m1;
    constexpr mat4 m2 (2.3f);

    constexpr mat4 m3 {
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

    pbrlib::testing::equality(m1, r1);
    pbrlib::testing::equality(m2, r2);
    pbrlib::testing::equality(m3, r3);
}

TEST(Mat4Tests, EqualAndNotEqual)
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

    pbrlib::testing::thisTrue(m1 == m2);

    m2.at(2, 3) = 23;

    pbrlib::testing::thisTrue(m1 != m2);
}

TEST(Mat4Tests, EqualAndNotEqualTypeFloat)
{
    constexpr mat4 m1 {
        1.23f, 3.32f, 34.5f, 4.43f,
        4.32f, 23.3f, 4.52f, 3.51f,
        43.0f, 5.03f, 0.06f, 7.23f,
        23.8f, 2.46f, 3.53f, 13.4f
    };

    mat4 m2 {
        1.23f, 3.32f, 34.5f, 4.43f,
        4.32f, 23.3f, 4.52f, 3.51f,
        43.0f, 5.03f, 0.06f, 7.23f,
        23.8f, 2.46f, 3.53f, 13.4f
    };

    pbrlib::testing::thisTrue(m1 == m2);

    m2.at(2, 3) = 23.23f;

    pbrlib::testing::thisTrue(m1 != m2);
}

TEST(Mat4Tests, AdditionAndSubtraction)
{
    constexpr Matrix4x4<short> m1 (23);
    constexpr Matrix4x4<short> m2 (37);

    Matrix4x4<short> res = m1 + m2;

    pbrlib::testing::equality(Matrix4x4<short>(60), res);

    res = m1 - m2;

    pbrlib::testing::equality(Matrix4x4<short>(-14), res);

    res =   m1;
    res +=  m2;

    pbrlib::testing::equality(Matrix4x4<short>(60), res);

    res -= m2;

    pbrlib::testing::equality(Matrix4x4<short>(23), res);
}

TEST(Mat4Tests, AdditionAndSubtractionTypeFloat)
{
    constexpr mat4 m1 (23.0f);
    constexpr mat4 m2 (37.0f);

    mat4 res = m1 + m2;

    pbrlib::testing::equality(mat4(60.0f), res);

    res = m1 - m2;

    pbrlib::testing::equality(mat4(-14.0f), res);

    res =   m1;
    res +=  m2;

    pbrlib::testing::equality(mat4(60.0f), res);

    res -= m2;

    pbrlib::testing::equality(mat4(23.0f), res);
}

TEST(Mat4Tests, ScalarMultiplication)
{
    constexpr Matrix4x4<int>    m (16);
    constexpr int               s (2);

    Matrix4x4<int> res = m * s;

    pbrlib::testing::equality(Matrix4x4<int>(32), res);

    res *= s;

    pbrlib::testing::equality(Matrix4x4<int>(64), res);
}

TEST(Mat4Tests, ScalarMultiplicationTypeFloat)
{
    constexpr mat4    m (16.5f);
    constexpr float               s (2.0f);

    mat4 res = m * s;

    pbrlib::testing::equality(mat4(33.0f), res);

    res *= s;

    pbrlib::testing::equality(mat4(66.0f), res);
}

TEST(Mat4Tests, MatrixMultiplication)
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

    pbrlib::testing::equality(r1, r2);

    r1 =     m1;
    r1 *=    m2;

    pbrlib::testing::equality(r1, r2);
}

TEST(Mat4Tests, MatrixMultiplicationTypeFloat)
{
    constexpr mat4 m1 {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    constexpr mat4 m2 {
        6.0f, 4.00f, 5.0f, 3.3f, 
        5.3f, 0.05f, 2.0f, 5.0f,
        7.0f, 5.00f, 3.0f, 2.0f,
        3.0f, 5.00f, 2.0f, 1.0f
    };

    mat4 r1 = m1 * m2;

    constexpr mat4 r2 {
        74.0999985f, 67.8000030f, 85.1999969f, 52.5999985f,
        50.5499992f, 48.7999992f, 48.2000008f, 35.3499985f,
        61.0000000f, 58.0000000f, 77.0000000f, 53.0000000f,
        45.0000000f, 40.0000000f, 55.0000000f, 33.0000000f
    };

    pbrlib::testing::equality(r1, r2);

    r1 =     m1;
    r1 *=    m2;

    pbrlib::testing::equality(r1, r2);
}

TEST(Mat4Tests, MatrixAndVectorMultiplication)
{
    constexpr Matrix4x4<int> m {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    constexpr ivec4 v   (2, 1, 3, 4);
    constexpr ivec4 r1  (50, 44, 46, 25);

    ivec4 r2 = m * v;

    pbrlib::testing::equality(r1, r2);
}

TEST(Mat4Tests, MatrixAndVectorMultiplicationTypeFloat)
{
    constexpr mat4 m {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    constexpr vec4 v    (2.2f, 23.14f, 3.34f, 4.04f);
    constexpr vec4 r1   (162.879990f, 134.559998f, 181.639999f, 92.9800034f);

    vec4 r2 = m * v;

    pbrlib::testing::equality(r1, r2);
}

TEST(Mat4Tests, AccessToElement)
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
            pbrlib::testing::equality(r[i * 4 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 4; i++) {
        for (size_t j{0}; j < 4; j++) {
            pbrlib::testing::equality(r[i * 4 + j], m.at(i, j));
        }
    }
}

TEST(Mat4Tests, AccessToElementTypeFloat)
{
    constexpr mat4 m {
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
            pbrlib::testing::equality(r[i * 4 + j], m[i][j]);
        }
    }

    for (size_t i{0}; i < 4; i++) {
        for (size_t j{0}; j < 4; j++) {
            pbrlib::testing::equality(r[i * 4 + j], m.at(i, j));
        }
    }
}

TEST(Mat4Tests, Determinant)
{
    constexpr Matrix4x4<int> m {
        1, 2, 3, 4,
        5, 4, 6, 3,
        5, 4, 6, 2,
        7, 6, 4, 2
    };

    constexpr int r = 30;

    pbrlib::testing::equality(r, m.det());
}

TEST(Mat4Tests, DeterminantTypeFloat)
{
    constexpr mat4 m {
        1.0f, 2.00f, 3.3f, 4.0f,
        5.0f, 4.00f, 6.0f, 3.0f,
        5.0f, 34.4f, 6.6f, 2.0f,
        7.0f, 6.00f, 4.0f, 2.0f
    };

    constexpr float r = 1614.28f;

    pbrlib::testing::equality(r, m.det());
}

TEST(Mat4Tests, Transpose)
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

    pbrlib::testing::equality(r, transpose(m));

    m.transpose();

    pbrlib::testing::equality(r, m);
}

TEST(Mat4Tests, TransposeTypeFloat)
{
    mat4 m {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 4.0f, 6.0f, 3.0f,
        5.0f, 4.0f, 6.0f, 2.0f,
        7.0f, 6.0f, 4.0f, 2.0f
    };

    constexpr mat4 r {
        1.0f, 5.0f, 5.0f, 7.0f, 
        2.0f, 4.0f, 4.0f, 6.0f, 
        3.0f, 6.0f, 6.0f, 4.0f, 
        4.0f, 3.0f, 2.0f, 2.0f
    };

    pbrlib::testing::equality(r, transpose(m));

    m.transpose();

    pbrlib::testing::equality(r, m);
}

TEST(Mat4Tests, Inverse)
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

    pbrlib::testing::equality(r, inverse(m));
}

TEST(Mat4Tests, InverseTypeFloat)
{
    constexpr mat4 m {
        1.0f, 3.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f,
        5.0f, 5.0f, 2.0f, 1.0f
    };

    constexpr mat4 r {
        -2.00000000f, 1.6923077100f, -1.30769229f, 1.000000000f,
        1.000000000f, -0.692307711f, 0.307692289f, 0.000000000f,
        3.999994280f, -4.076917650f, 3.923071620f, -2.99999619f,
        -2.99999619f, 3.1538419700f, -2.84615016f, 1.999997380f
    };

    pbrlib::testing::equality(r, inverse(m));
}