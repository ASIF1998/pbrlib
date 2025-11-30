#include "testing_types.hpp"

#include <pbrlib/math/matrix3x3.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/lerp.hpp>

using namespace pbrlib::math;

template <typename T>
class Mat3Tests 
    : public ::testing::Test 
{ };

TYPED_TEST_SUITE(Mat3Tests, MathTestingTypes);

TYPED_TEST(Mat3Tests, Ctor)
{
    constexpr Matrix3x3<TypeParam>  m1;
    constexpr Matrix3x3             m2 (static_cast<TypeParam>(23));
    constexpr Matrix3x3             m3 (
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
        static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(3), static_cast<TypeParam>(4), static_cast<TypeParam>(5)
    );

    constexpr TypeParam r1[9] {
        static_cast<TypeParam>(1), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(1), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(1)
    };

    constexpr TypeParam r2[9] {
        static_cast<TypeParam>(23), static_cast<TypeParam>(0), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(23), static_cast<TypeParam>(0),
        static_cast<TypeParam>(0), static_cast<TypeParam>(0), static_cast<TypeParam>(23)
    };

    constexpr TypeParam r3[9] {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
        static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(3), static_cast<TypeParam>(4), static_cast<TypeParam>(5)
    };

    pbrlib::testing::equality(m1, r1);
    pbrlib::testing::equality(m2, r2);
    pbrlib::testing::equality(m3, r3);
}

TYPED_TEST(Mat3Tests, EqualAndNotEqual)
{
    constexpr Matrix3x3 m1 (
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
        static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(3), static_cast<TypeParam>(4), static_cast<TypeParam>(5)
    );

    Matrix3x3 m2 (
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3),
        static_cast<TypeParam>(2), static_cast<TypeParam>(3), static_cast<TypeParam>(4),
        static_cast<TypeParam>(3), static_cast<TypeParam>(4), static_cast<TypeParam>(5)
    );

    pbrlib::testing::thisTrue(m1 == m2);

    m2.at(1, 2) = static_cast<TypeParam>(60);
    pbrlib::testing::thisTrue(m1 != m2);
}

TYPED_TEST(Mat3Tests, AdditionAndSubtraction)
{
    constexpr Matrix3x3 m1 (static_cast<TypeParam>(23));
    constexpr Matrix3x3 m2 (static_cast<TypeParam>(37));

    auto res = m1 + m2;
    pbrlib::testing::equality(Matrix3x3(static_cast<TypeParam>(60)), res);

    res = m1 - m2;
    pbrlib::testing::equality(Matrix3x3(static_cast<TypeParam>(-14)), res);

    res =   m1;
    res +=  m2;
    pbrlib::testing::equality(Matrix3x3(static_cast<TypeParam>(60)), res);

    res -= m2;
    pbrlib::testing::equality(Matrix3x3(static_cast<TypeParam>(23)), res);
}

TYPED_TEST(Mat3Tests, ScalarMultiplication)
{
    constexpr Matrix3x3 m (static_cast<TypeParam>(16));

    constexpr auto s = static_cast<TypeParam>(2);

    auto res = m * s;
    pbrlib::testing::equality(Matrix3x3(static_cast<TypeParam>(32)), res);
    
    res *= s;
    pbrlib::testing::equality(Matrix3x3(static_cast<TypeParam>(64)), res);
}

TYPED_TEST(Mat3Tests, MatrixMultiplication)
{
    constexpr Matrix3x3 m1 {
        static_cast<TypeParam>(13), 
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(3),
        static_cast<TypeParam>(76), 
        static_cast<TypeParam>(5), 
        static_cast<TypeParam>(3),
        static_cast<TypeParam>(8), 
        static_cast<TypeParam>(9), 
        static_cast<TypeParam>(2)
    };

    constexpr Matrix3x3 m2 {
        static_cast<TypeParam>(5), 
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(8),
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(1),
        static_cast<TypeParam>(54), 
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(8)
    };

    auto r = m1 * m2;
    constexpr Matrix3x3 res {
        static_cast<TypeParam>(585), 
        static_cast<TypeParam>(112), 
        static_cast<TypeParam>(49),
        static_cast<TypeParam>(110), 
        static_cast<TypeParam>(18), 
        static_cast<TypeParam>(11),
        static_cast<TypeParam>(1222), 
        static_cast<TypeParam>(210), 
        static_cast<TypeParam>(196)  
    };

    pbrlib::testing::equality(r, res);

    r =     m1;
    r *=    m2;
    pbrlib::testing::equality(r, res);
}

TYPED_TEST(Mat3Tests, MatrixAndVectorMultiplication)
{
    constexpr Matrix3x3 m {
        static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3), 
        static_cast<TypeParam>(6), static_cast<TypeParam>(4), static_cast<TypeParam>(3),
        static_cast<TypeParam>(9), static_cast<TypeParam>(7), static_cast<TypeParam>(3)
    };

    constexpr Vec3<TypeParam> v (
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(23)
    );
    
    constexpr Vec3<TypeParam> res (
        static_cast<TypeParam>(226), 
        static_cast<TypeParam>(175), 
        static_cast<TypeParam>(81)
    );

    pbrlib::testing::equality(res, m * v);
}   

TYPED_TEST(Mat3Tests, AccessToElement)
{
    constexpr Matrix3x3 m {
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(4), 
        static_cast<TypeParam>(3),
        static_cast<TypeParam>(9), 
        static_cast<TypeParam>(7), 
        static_cast<TypeParam>(3)
    };

    constexpr TypeParam r[9] {
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(4), 
        static_cast<TypeParam>(3),
        static_cast<TypeParam>(9), 
        static_cast<TypeParam>(7), 
        static_cast<TypeParam>(3)
    };

    for (size_t i = 0; i < 3; i++) 
    {
        for (size_t j = 0; j < 3; j++)
            pbrlib::testing::equality(r[i * 3 + j], m[i][j]);
    }

    for (size_t i = 0; i < 3; i++) 
    {
        for (size_t j = 0; j < 3; j++)
            pbrlib::testing::equality(r[i * 3 + j], m.at(i, j));
    }
}

TYPED_TEST(Mat3Tests, Determinant)
{
    constexpr Matrix3x3 m {
        static_cast<TypeParam>(2), 
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(5),
        static_cast<TypeParam>(3), 
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(7),
        static_cast<TypeParam>(1), 
        static_cast<TypeParam>(6), 
        static_cast<TypeParam>(9)
    };
    
    constexpr TypeParam r = static_cast<TypeParam>(64);

    pbrlib::testing::equality(r, m.det());
}

TYPED_TEST(Mat3Tests, Transpose)
{
    constexpr Matrix3x3 res {
        pbrlib::testing::roundForInteger<TypeParam>(1.0), 
        pbrlib::testing::roundForInteger<TypeParam>(2.0), 
        pbrlib::testing::roundForInteger<TypeParam>(3.5),
        pbrlib::testing::roundForInteger<TypeParam>(2.2), 
        pbrlib::testing::roundForInteger<TypeParam>(3.0), 
        pbrlib::testing::roundForInteger<TypeParam>(1.2),
        pbrlib::testing::roundForInteger<TypeParam>(8.5), 
        pbrlib::testing::roundForInteger<TypeParam>(2.6), 
        pbrlib::testing::roundForInteger<TypeParam>(12.3)
    };

    Matrix3x3 m {
        pbrlib::testing::roundForInteger<TypeParam>(1.0), 
        pbrlib::testing::roundForInteger<TypeParam>(2.2), 
        pbrlib::testing::roundForInteger<TypeParam>(8.50),
        pbrlib::testing::roundForInteger<TypeParam>(2.0), 
        pbrlib::testing::roundForInteger<TypeParam>(3.0), 
        pbrlib::testing::roundForInteger<TypeParam>(2.60),
        pbrlib::testing::roundForInteger<TypeParam>(3.5), 
        pbrlib::testing::roundForInteger<TypeParam>(1.2), 
        pbrlib::testing::roundForInteger<TypeParam>(12.3)
    };

    pbrlib::testing::equality(res, transpose(m));

    m.transpose();
    pbrlib::testing::equality(res, m);
}

TYPED_TEST(Mat3Tests, Inverse)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Matrix3x3 res {
            static_cast<TypeParam>(1.0), 
            static_cast<TypeParam>(2.0), 
            static_cast<TypeParam>(3.5),
            static_cast<TypeParam>(2.2), 
            static_cast<TypeParam>(3.0), 
            static_cast<TypeParam>(1.2),
            static_cast<TypeParam>(8.5), 
            static_cast<TypeParam>(2.6), 
            static_cast<TypeParam>(12.3)
        };
    
        Matrix3x3 m {
            static_cast<TypeParam>(-0.488362006), 
            static_cast<TypeParam>(0.224085586236808), 
            static_cast<TypeParam>(0.117102790226977),
            static_cast<TypeParam>(0.2437472790), 
            static_cast<TypeParam>(0.252276998698858), 
            static_cast<TypeParam>(-0.09397136420000),
            static_cast<TypeParam>(0.2859621050), 
            static_cast<TypeParam>(-0.20818272200000), 
            static_cast<TypeParam>(0.020239988300000)
        };
    
        pbrlib::testing::equality(res, inverse(m));
    }
}

TYPED_TEST(Mat3Tests, AtMethodEdgeCase)
{
    constexpr Matrix3x3 mat {
        pbrlib::testing::roundForInteger<TypeParam>(1.0), 
        pbrlib::testing::roundForInteger<TypeParam>(2.0), 
        pbrlib::testing::roundForInteger<TypeParam>(3.5),
        pbrlib::testing::roundForInteger<TypeParam>(2.2), 
        pbrlib::testing::roundForInteger<TypeParam>(3.0), 
        pbrlib::testing::roundForInteger<TypeParam>(1.2),
        pbrlib::testing::roundForInteger<TypeParam>(8.5), 
        pbrlib::testing::roundForInteger<TypeParam>(2.6), 
        pbrlib::testing::roundForInteger<TypeParam>(12.3)
    };

    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(3, 0);
    }, pbrlib::exception::InvalidArgument);
    
    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(0, 3);
    }, pbrlib::exception::InvalidArgument);
    
    EXPECT_THROW({
        [[maybe_unused]] const auto p = mat.at(3, 3);
    }, pbrlib::exception::InvalidArgument);
}

TYPED_TEST(Mat3Tests, Lerp)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Matrix3x3 result (pbrlib::testing::roundForInteger<TypeParam>(0.75));
        pbrlib::testing::equality(result, lerp (
            Matrix3x3(static_cast<TypeParam>(0.0)), 
            Matrix3x3(static_cast<TypeParam>(1.0)), 
            pbrlib::testing::roundForInteger<TypeParam>(0.75))
        );
    }
}