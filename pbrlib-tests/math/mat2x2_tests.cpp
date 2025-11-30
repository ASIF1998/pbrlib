#include "testing_types.hpp"

#include <pbrlib/math/matrix2x2.hpp>
#include <pbrlib/math/lerp.hpp>
#include <pbrlib/math/concepts.hpp>

#include <type_traits>

using namespace pbrlib::math;

template <typename T>
class Mat2Tests 
    : public ::testing::Test 
{ };

TYPED_TEST_SUITE(Mat2Tests, MathTestingTypes);

TYPED_TEST(Mat2Tests, Ctor)
{
    constexpr TypeParam r1[4] {
        static_cast<TypeParam>(1), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(1)
    };

    constexpr TypeParam r2[4] {
        static_cast<TypeParam>(23), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(23)
    };

    constexpr TypeParam r3[4] {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), 
        static_cast<TypeParam>(3), static_cast<TypeParam>(23)
    };

    constexpr Matrix2x2<TypeParam>  m1;
    constexpr Matrix2x2             m2(static_cast<TypeParam>(23));
    constexpr Matrix2x2             m3 (
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(23)
    );

    pbrlib::testing::equality(m1, r1);
    pbrlib::testing::equality(m2, r2);
    pbrlib::testing::equality(m3, r3);  
}

TYPED_TEST(Mat2Tests, EqualAndNotEqual)
{
    constexpr Matrix2x2 m1 (
        pbrlib::testing::roundForInteger<TypeParam>(1.12), 
        pbrlib::testing::roundForInteger<TypeParam>(12.2), 
        pbrlib::testing::roundForInteger<TypeParam>(45.345), 
        pbrlib::testing::roundForInteger<TypeParam>(4.545)
    );

    Matrix2x2 m2 (
        pbrlib::testing::roundForInteger<TypeParam>(1.12), 
        pbrlib::testing::roundForInteger<TypeParam>(12.2), 
        pbrlib::testing::roundForInteger<TypeParam>(45.345), 
        pbrlib::testing::roundForInteger<TypeParam>(4.545)
    );

    pbrlib::testing::thisTrue(m1 == m2);

    m2.at(0, 1) = static_cast<TypeParam>(6);

    pbrlib::testing::thisTrue(m1 != m2);
}

TYPED_TEST(Mat2Tests, AdditionAndSubtraction)
{
    constexpr Matrix2x2 m1(static_cast<TypeParam>(3));
    constexpr Matrix2x2 m2(static_cast<TypeParam>(4));

    auto res = m1 + m2;
    pbrlib::testing::equality(Matrix2x2(static_cast<TypeParam>(7)), res);

    res = m2 - m1;
    pbrlib::testing::equality(Matrix2x2(static_cast<TypeParam>(1)), res);

    res += m1;
    pbrlib::testing::equality(res, m2);

    res += m2;
    res -= m2;
    pbrlib::testing::equality(res, m2);
}

TYPED_TEST(Mat2Tests, ScalarMultiplication)
{
    constexpr Matrix2x2 m (static_cast<TypeParam>(17));

    constexpr auto s = static_cast<TypeParam>(5);

    auto res = m * s;
    pbrlib::testing::equality(Matrix2x2(static_cast<TypeParam>(85)), res);

    res *= s;
    pbrlib::testing::equality(Matrix2x2(static_cast<TypeParam>(425)), res);
}

TYPED_TEST(Mat2Tests, MatrixMultiplication)
{
    constexpr Matrix2x2 res1 (
        static_cast<TypeParam>(115), 
        static_cast<TypeParam>(1885),
        static_cast<TypeParam>(30), 
        static_cast<TypeParam>(270)
    );

    constexpr Matrix2x2 m1 (
        static_cast<TypeParam>(12), 
        static_cast<TypeParam>(4), 
        static_cast<TypeParam>(43), 
        static_cast<TypeParam>(6)
    );

    constexpr Matrix2x2 m2 (
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(43)
    );

    auto res2 = m1 * m2;
    pbrlib::testing::equality(res1, res2);

    res2 = m1;
    res2 *= m2;
    pbrlib::testing::equality(res1, res2);
}

TYPED_TEST(Mat2Tests, MatrixAndVectorMultiplication)
{
    constexpr Vec2 res1 (
        static_cast<TypeParam>(103), 
        static_cast<TypeParam>(247)
    );

    constexpr Matrix2x2 m (
        static_cast<TypeParam>(13), 
        static_cast<TypeParam>(32), 
        static_cast<TypeParam>(5), 
        static_cast<TypeParam>(11)
    );

    constexpr Vec2 v (
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(5)
    );

    auto res2 = m * v;
    pbrlib::testing::equality(res1, res2);
}

TYPED_TEST(Mat2Tests, AccessToElement)
{
    constexpr Matrix2x2 m (
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(4)
    );

    constexpr TypeParam r[4] {
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(4)
    };

    for (size_t i = 0; i < 2; i++) 
    {
        for (size_t j = 0; j < 2; j++)
            pbrlib::testing::equality(r[i * 2 + j], m[i][j]);
    }

    for (size_t i = 0; i < 2; i++) 
    {
        for (size_t j = 0; j < 2; j++)
            pbrlib::testing::equality(r[i * 2 + j], m.at(i, j));
    }
}

TYPED_TEST(Mat2Tests, Determinant)
{
    constexpr Matrix2x2 m (
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(4)
    );

    constexpr auto r = static_cast<TypeParam>(5);

    pbrlib::testing::equality(r, m.det());
}

TYPED_TEST(Mat2Tests, Transpose)
{
    Matrix2x2 m (
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(4)
    );

    constexpr Matrix2x2 res1 (
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(4)
    );

    auto res2 = transpose(m);
    pbrlib::testing::equality(res1, res2);

    m.transpose();
    pbrlib::testing::equality(res1, m);
}

TYPED_TEST(Mat2Tests, Inverse)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Matrix2x2 res1 (
            static_cast<TypeParam>(1), 
            static_cast<TypeParam>(0), 
            static_cast<TypeParam>(-2), 
            static_cast<TypeParam>(1)
        );
    
        Matrix2x2 m (
            static_cast<TypeParam>(1), 
            static_cast<TypeParam>(2), 
            static_cast<TypeParam>(0), 
            static_cast<TypeParam>(1)
        );
    
        auto res2 = inverse(m);
        pbrlib::testing::equality(res1, res2);
    
        m.inverse();
        pbrlib::testing::equality(res1, m);
    }
}

TYPED_TEST(Mat2Tests, AtMethodEdgeCase)
{
    constexpr Matrix2x2 mat (
        static_cast<TypeParam>(-2.0), 
        static_cast<TypeParam>(1.0), 
        static_cast<TypeParam>(1.5), 
        static_cast<TypeParam>(-0.5)
    );

    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(2, 0);
    }, pbrlib::exception::InvalidArgument);
    
    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(0, 2);
    }, pbrlib::exception::InvalidArgument);

    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(2, 2);
    }, pbrlib::exception::InvalidArgument);
}

TYPED_TEST(Mat2Tests, Lerp)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Matrix2x2 result (pbrlib::testing::roundForInteger<TypeParam>(0.75));
        pbrlib::testing::equality(result, lerp (
            Matrix2x2(static_cast<TypeParam>(0.0)), 
            Matrix2x2(static_cast<TypeParam>(1.0)), 
            pbrlib::testing::roundForInteger<TypeParam>(0.75))
        );
    }
}