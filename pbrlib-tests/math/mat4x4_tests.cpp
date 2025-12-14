#include "testing_types.hpp"

#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/lerp.hpp>

using namespace pbrlib::math;

template <typename T>
class Mat4Tests 
    : public ::testing::Test 
{ };

TYPED_TEST_SUITE(Mat4Tests, MathTestingTypes);

TYPED_TEST(Mat4Tests, Ctor)
{
    constexpr Matrix4x4<TypeParam> m1;
    constexpr Matrix4x4 m2 (static_cast<TypeParam>(23));

    constexpr Matrix4x4 m3 {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(4), static_cast<TypeParam>(3), static_cast<TypeParam>(2), static_cast<TypeParam>(1),
        static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
        static_cast<TypeParam>(8), static_cast<TypeParam>(6), static_cast<TypeParam>(3), static_cast<TypeParam>(1)
    };

    constexpr TypeParam r1[16] {
        static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0), 
        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1)
    };

    constexpr TypeParam r2[16] {
        static_cast<TypeParam>(23), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(23), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(23), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(23)
    };

    constexpr TypeParam r3[16] {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(4), static_cast<TypeParam>(3), static_cast<TypeParam>(2), static_cast<TypeParam>(1),
        static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7),
        static_cast<TypeParam>(8), static_cast<TypeParam>(6), static_cast<TypeParam>(3), static_cast<TypeParam>(1)
    };

    pbrlib::testing::equality(m1, r1);
    pbrlib::testing::equality(m2, r2);
    pbrlib::testing::equality(m3, r3);
}

TYPED_TEST(Mat4Tests, EqualAndNotEqual)
{
    constexpr Matrix4x4 m1 {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(3), static_cast<TypeParam>(2), static_cast<TypeParam>(5), static_cast<TypeParam>(6), 
        static_cast<TypeParam>(1), static_cast<TypeParam>(9), static_cast<TypeParam>(5), static_cast<TypeParam>(8),
        static_cast<TypeParam>(1), static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(4)
    };

    Matrix4x4 m2 {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(3), static_cast<TypeParam>(2), static_cast<TypeParam>(5), static_cast<TypeParam>(6), 
        static_cast<TypeParam>(1), static_cast<TypeParam>(9), static_cast<TypeParam>(5), static_cast<TypeParam>(8),
        static_cast<TypeParam>(1), static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(4)
    };

    pbrlib::testing::thisTrue(m1 == m2);

    m2.at(2, 3) = 23;

    pbrlib::testing::thisTrue(m1 != m2);
}

TYPED_TEST(Mat4Tests, AdditionAndSubtraction)
{
    constexpr Matrix4x4 m1 (static_cast<TypeParam>(23));
    constexpr Matrix4x4 m2 (static_cast<TypeParam>(37));

    auto res = m1 + m2;

    pbrlib::testing::equality(Matrix4x4(static_cast<TypeParam>(60)), res);

    res = m1 - m2;

    pbrlib::testing::equality(Matrix4x4(static_cast<TypeParam>(-14)), res);

    res =   m1;
    res +=  m2;

    pbrlib::testing::equality(Matrix4x4(static_cast<TypeParam>(60)), res);

    res -= m2;

    pbrlib::testing::equality(Matrix4x4(static_cast<TypeParam>(23)), res);
}

TYPED_TEST(Mat4Tests, ScalarMultiplication)
{
    constexpr Matrix4x4 m (static_cast<TypeParam>(17));

    constexpr auto s = static_cast<TypeParam>(5);

    pbrlib::testing::equality(Matrix2x2(static_cast<TypeParam>(85)), m * s);
    pbrlib::testing::equality(Matrix2x2(static_cast<TypeParam>(85)), s * m);

    auto m2 = m;
    m2 *= s;

    pbrlib::testing::equality(Matrix2x2(static_cast<TypeParam>(85)), m2);
}

TYPED_TEST(Mat4Tests, MatrixMultiplication)
{
    constexpr Matrix4x4 m1 {
        static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(5), static_cast<TypeParam>(3), 
        static_cast<TypeParam>(5), static_cast<TypeParam>(3), static_cast<TypeParam>(2), static_cast<TypeParam>(5),
        static_cast<TypeParam>(7), static_cast<TypeParam>(5), static_cast<TypeParam>(3), static_cast<TypeParam>(2),
        static_cast<TypeParam>(3), static_cast<TypeParam>(5), static_cast<TypeParam>(2), static_cast<TypeParam>(1)
    };

    constexpr Matrix4x4 m2 {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(3),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(2),
        static_cast<TypeParam>(7), static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(2)
    };

    auto r1 = m1 * m2;

    constexpr Matrix4x4 r2 {
        static_cast<TypeParam>(49),  static_cast<TypeParam>(45), static_cast<TypeParam>(26), static_cast<TypeParam>(23),
        static_cast<TypeParam>(101), static_cast<TypeParam>(77), static_cast<TypeParam>(57), static_cast<TypeParam>(50),
        static_cast<TypeParam>(98),  static_cast<TypeParam>(72), static_cast<TypeParam>(55), static_cast<TypeParam>(49),
        static_cast<TypeParam>(106), static_cast<TypeParam>(76), static_cast<TypeParam>(63), static_cast<TypeParam>(61)
    };

    pbrlib::testing::equality(r1, r2);

    r1 =     m1;
    r1 *=    m2;

    pbrlib::testing::equality(r1, r2);
}

TYPED_TEST(Mat4Tests, MatrixAndVectorMultiplication)
{
    constexpr Matrix4x4 m {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(3),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(2),
        static_cast<TypeParam>(7), static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(2)
    };

    constexpr Vec4 v   (static_cast<TypeParam>(2), static_cast<TypeParam>(1), static_cast<TypeParam>(3), static_cast<TypeParam>(4));
    constexpr Vec4 r1  (static_cast<TypeParam>(50), static_cast<TypeParam>(44), static_cast<TypeParam>(46), static_cast<TypeParam>(25));

    auto r2 = m * v;

    pbrlib::testing::equality(r1, r2);
}

TYPED_TEST(Mat4Tests, AccessToElement)
{
    constexpr Matrix4x4 m {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(3),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(2),
        static_cast<TypeParam>(7), static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(2)
    };

    constexpr TypeParam r[16] {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(3),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(2),
        static_cast<TypeParam>(7), static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(2)
    };

    for (size_t i =0; i < 4; i++) 
    {
        for (size_t j = 0; j < 4; j++)
            pbrlib::testing::equality(r[i * 4 + j], m[i][j]);
    }

    for (size_t i = 0; i < 4; i++) 
    {
        for (size_t j = 0; j < 4; j++)
            pbrlib::testing::equality(r[i * 4 + j], m.at(i, j));
    }
}

TYPED_TEST(Mat4Tests, Determinant)
{
    constexpr Matrix4x4 m {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(3),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(2),
        static_cast<TypeParam>(7), static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(2)
    };

    constexpr auto r = static_cast<TypeParam>(30);

    pbrlib::testing::equality(r, m.det());
}

TYPED_TEST(Mat4Tests, Transpose)
{
    Matrix4x4 m {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(3),
        static_cast<TypeParam>(5), static_cast<TypeParam>(4), static_cast<TypeParam>(6), static_cast<TypeParam>(2),
        static_cast<TypeParam>(7), static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(2)
    };

    constexpr Matrix4x4 r {
        static_cast<TypeParam>(1), static_cast<TypeParam>(5), static_cast<TypeParam>(5), static_cast<TypeParam>(7), 
        static_cast<TypeParam>(2), static_cast<TypeParam>(4), static_cast<TypeParam>(4), static_cast<TypeParam>(6), 
        static_cast<TypeParam>(3), static_cast<TypeParam>(6), static_cast<TypeParam>(6), static_cast<TypeParam>(4), 
        static_cast<TypeParam>(4), static_cast<TypeParam>(3), static_cast<TypeParam>(2), static_cast<TypeParam>(2)
    };

    pbrlib::testing::equality(r, transpose(m));

    m.transpose();

    pbrlib::testing::equality(r, m);
}

TYPED_TEST(Mat4Tests, Inverse)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Matrix4x4 m {
            static_cast<TypeParam>(1), static_cast<TypeParam>(3), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
            static_cast<TypeParam>(5), static_cast<TypeParam>(6), static_cast<TypeParam>(7), static_cast<TypeParam>(8),
            static_cast<TypeParam>(8), static_cast<TypeParam>(7), static_cast<TypeParam>(6), static_cast<TypeParam>(5),
            static_cast<TypeParam>(5), static_cast<TypeParam>(5), static_cast<TypeParam>(2), static_cast<TypeParam>(1)
        };
    
        constexpr Matrix4x4 r {
            static_cast<TypeParam>(-2), static_cast<TypeParam>(1.6923), static_cast<TypeParam>(-1.3076), static_cast<TypeParam>(1), 
            static_cast<TypeParam>(1), static_cast<TypeParam>(-0.6923), static_cast<TypeParam>(0.3076), static_cast<TypeParam>(0),
            static_cast<TypeParam>(4), static_cast<TypeParam>(-4.0769), static_cast<TypeParam>(3.9230), static_cast<TypeParam>(-3),
            static_cast<TypeParam>(-3), static_cast<TypeParam>(3.1538), static_cast<TypeParam>(-2.8461), static_cast<TypeParam>(2)
        };
    
        pbrlib::testing::equality(r, inverse(m));
    }
}

TYPED_TEST(Mat4Tests, AtMethodEdgeCase)
{
    constexpr Matrix4x4 mat (static_cast<TypeParam>(23));

    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(4, 0);
    }, pbrlib::exception::InvalidArgument);
    
    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(0, 4);
    }, pbrlib::exception::InvalidArgument);
    
    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(4, 4);
    }, pbrlib::exception::InvalidArgument);
}

TYPED_TEST(Mat4Tests, Lerp)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Matrix4x4 result (pbrlib::testing::roundForInteger<TypeParam>(0.75));
        pbrlib::testing::equality(result, lerp (
            Matrix4x4(static_cast<TypeParam>(0.0)), 
            Matrix4x4(static_cast<TypeParam>(1.0)), 
            pbrlib::testing::roundForInteger<TypeParam>(0.75))
        );
    }
}