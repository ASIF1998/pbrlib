#include "../utils.hpp"

#include <pbrlib/math/matrix2x2.hpp>

using namespace pbrlib::math;

TEST(Mat2Tests, Constructor)
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

TEST(Mat2Tests, ConstructorTypeFloat)
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
    
    constexpr mat2 m1;
    constexpr mat2 m2(2.3f);
    constexpr mat2 m3(1.2f, 2.1f, 5.34f, 323.4f);

#if (defined(__SSE__) || defined(__AVX2__))
    mat2 m4(r4.simd);
#endif

    mat2 m5(r3);

    pbrlib::testing::utils::equality(m1, r1);
    pbrlib::testing::utils::equality(m2, r2);
    pbrlib::testing::utils::equality(m3, r3);

#if (defined(__SSE__) || defined(__AVX2__))
    pbrlib::testing::utils::equality(m4, r4.simd);
#endif

    pbrlib::testing::utils::equality(m5, r3);
}

TEST(Mat2Tests, EqualAndNotEqual)
{
    constexpr Matrix2x2<short>  m1 (1, 2, 3, 4);
    Matrix2x2<short>            m2 (1, 2, 3, 4);

    pbrlib::testing::utils::thisTrue(m1 == m2);

    m2.at(0, 1) = 6;

    pbrlib::testing::utils::thisTrue(m1 != m2);
}

TEST(Mat2Tests, EqualAndNotEqualTypeFloat)
{
    constexpr mat2  m1 (1.12f, 12.2f, 45.345f, 4.545f);
    mat2            m2 (1.12f, 12.2f, 45.345f, 4.545f);

    pbrlib::testing::utils::thisTrue(m1 == m2);

    m2.at(0, 1) = 0.43f;

    pbrlib::testing::utils::thisTrue(m1 != m2);
}

TEST(Mat2Tests, AdditionAndSubtraction)
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

TEST(Mat2Tests, AdditionAndSubtractionTypeFloat)
{
    constexpr mat2 m1(2.5f);
    constexpr mat2 m2(3.0f);

    mat2 res = m1 + m2;

    pbrlib::testing::utils::equality(mat2(5.5f), res);

    res = m1 - m2;

    pbrlib::testing::utils::equality(mat2(-0.5f), res);

    res += m2;

    pbrlib::testing::utils::equality(res, m1);

    res += m2;
    res -= m2;

    pbrlib::testing::utils::equality(res, m1);
}

TEST(Mat2Tests, ScalarMultiplication)
{
    constexpr Matrix2x2<int>    m   (32);
    constexpr int               s   (4);

    Matrix2x2<int> res (m * s);

    pbrlib::testing::utils::equality(Matrix2x2<int>(128), res);
    
    res *= s;

    pbrlib::testing::utils::equality(Matrix2x2<int>(512), res);
}

TEST(Mat2Tests, ScalarMultiplicationTypeFloat)
{
    constexpr mat2  m   (16.5f);
    constexpr float             s   (4.5f);

    mat2 res = m * s;

    pbrlib::testing::utils::equality(mat2(74.25f), res);

    res *= (1.0f / s);

    pbrlib::testing::utils::equality(mat2(16.5f), res);
}

TEST(Mat2Tests, MatrixMultiplication)
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

TEST(Mat2Tests, MatrixMultiplicationTypeFloat)
{
    constexpr mat2 res1 (
        34.3500023f, 56.4490051f,
        36.6135025f, 4.08415985f
    );

    constexpr mat2 m1 (4.3f, 0.012f, 6.0f, 7.87f);
    constexpr mat2 m2 (1.5f, 4.65f, 12.43f, 0.5f);

    mat2 res2 = m1 * m2;

    pbrlib::testing::utils::equality(res1, res2);

    res2 = m1;
    res2 *= m2;

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(Mat2Tests, MatrixAndVectorMultiplication)
{
    constexpr ivec2 res1 (13, 14);

    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr ivec2             v (2, 3);

    ivec2 res2 = m * v;

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(Mat2Tests, MatrixAndVectorMultiplicationTypeFloat)
{
    constexpr vec2 res1 (43.0760002f, 11.8300009f);

    constexpr mat2 m (1.5f, 4.65f, 12.43f, 0.5f);
    constexpr vec2 v (2.2f, 3.2f);

    vec2 res2 = m * v;

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(Mat2Tests, AccessToElement)
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

TEST(Mat2Tests, AccessToElementTypeFloat)
{
    constexpr mat2    m (0.0065f, 1.23f, 23.3f, 23.454f);
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

TEST(Mat2Tests, Determinant)
{
    constexpr Matrix2x2<int>    m (2, 1, 3, 4);
    constexpr int               r (5);

    pbrlib::testing::utils::equality(r, m.det());
}

TEST(Mat2Tests, DeterminantTypeFloat)
{
    constexpr mat2  m (0.0065f, 1.23f, 23.3f, 23.454f);
    constexpr float             r (-28.506549f);

    pbrlib::testing::utils::equality(r, m.det());
}

TEST(Mat2Tests, Transpose)
{
    Matrix2x2<int>  m (2, 1, 3, 4);

    constexpr Matrix2x2<int>    res1 (2, 3, 1, 4);
    Matrix2x2<int>              res2 (transpose(m));

    pbrlib::testing::utils::equality(res1, res2);

    m.transpose();

    pbrlib::testing::utils::equality(res1, m);
}

TEST(Mat2Tests, TransposeTypeFloat)
{
    mat2 m (2.2f, 1.2f, 2.3f, 4.4f);

    constexpr mat2  res1 (2.2f, 2.3f, 1.2f, 4.4f);
    mat2            res2 (transpose(m));

    pbrlib::testing::utils::equality(res1, res2);

    m.transpose();

    pbrlib::testing::utils::equality(res1, res2);
}

TEST(Mat2Tests, Inverse)
{
    constexpr Matrix2x2<int> res1 (1, -2, 0, 1);

    Matrix2x2<int>  m (1, 2, 0, 1);

    Matrix2x2<int> res2 = inverse(m);

    pbrlib::testing::utils::equality(res1, res2);

    m.inverse();

    pbrlib::testing::utils::equality(res1, m);
}

TEST(Mat2Tests, InverseTypeFloat)
{
    constexpr mat2 res1 (-2.0f, 1.0f, 1.5f, -0.5f);

    mat2  m (1.0f, 2.0f, 3.0f, 4.0f);

    mat2 res2 = inverse(m);

    pbrlib::testing::utils::equality(res1, res2);

    m.inverse();

    pbrlib::testing::utils::equality(res1, res2);
}