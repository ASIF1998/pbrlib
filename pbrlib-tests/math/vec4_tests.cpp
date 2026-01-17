#include "testing_types.hpp"

#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/lerp.hpp>

using namespace pbrlib::math;

template <typename T>
class Vec4Tests
    : public ::testing::Test
{ };

TYPED_TEST_SUITE(Vec4Tests, MathTestingTypes);

TYPED_TEST(Vec4Tests, EqualAndNotEqual)
{
    constexpr Vec4 v1 (static_cast<TypeParam>(1));
    constexpr Vec4 v2 (static_cast<TypeParam>(1));
    constexpr Vec4 v3 (static_cast<TypeParam>(2));

    pbrlib::testing::equality(v1, v2);
    pbrlib::testing::notEquality(v1, v3);
}

TYPED_TEST(Vec4Tests, AdditionAndSubtraction)
{
    constexpr Vec4 v1 (
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(5),
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(7)
    );

    constexpr Vec4 v2 (
        static_cast<TypeParam>(4),
        static_cast<TypeParam>(7),
        static_cast<TypeParam>(10),
        static_cast<TypeParam>(8)
    );

    constexpr Vec4 r1 (
        static_cast<TypeParam>(6),
        static_cast<TypeParam>(12),
        static_cast<TypeParam>(12),
        static_cast<TypeParam>(15)
    );

    constexpr Vec4 r2 (
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(8),
        static_cast<TypeParam>(1)
    );

    pbrlib::testing::equality(r1, v2 + v1);
    pbrlib::testing::equality(r2, v2 - v1);
}

TYPED_TEST(Vec4Tests, Multiplication)
{
    Vec4 v (
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(3),
        static_cast<TypeParam>(6),
        static_cast<TypeParam>(9)
    );

    constexpr Vec4 res (
        static_cast<TypeParam>(4),
        static_cast<TypeParam>(6),
        static_cast<TypeParam>(12),
        static_cast<TypeParam>(18)
    );

    constexpr auto s = static_cast<TypeParam>(2);

    pbrlib::testing::equality(res, v * s);
    pbrlib::testing::equality(res, s * v);

    v *= s;

    pbrlib::testing::equality(res, v);
}

TYPED_TEST(Vec4Tests, AccessToElement)
{
    constexpr Vec4  v (
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(4),
        static_cast<TypeParam>(54),
        static_cast<TypeParam>(10)
    );

    pbrlib::testing::equality(static_cast<TypeParam>(2), v[0]);
    pbrlib::testing::equality(static_cast<TypeParam>(4), v[1]);
    pbrlib::testing::equality(static_cast<TypeParam>(54), v[2]);
    pbrlib::testing::equality(static_cast<TypeParam>(10), v[3]);

    pbrlib::testing::equality(static_cast<TypeParam>(2), v.x);
    pbrlib::testing::equality(static_cast<TypeParam>(4), v.y);
    pbrlib::testing::equality(static_cast<TypeParam>(54), v.z);
    pbrlib::testing::equality(static_cast<TypeParam>(10), v.w);
}

TYPED_TEST(Vec4Tests, Length)
{
    constexpr Vec4 v (
        static_cast<TypeParam>(1),
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(3),
        static_cast<TypeParam>(4)
    );

    pbrlib::testing::equality (
        pbrlib::testing::roundForInteger<TypeParam>(5.47722578),
        v.length()
    );
}

TYPED_TEST(Vec4Tests, Normalize)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec4  v1 (
            static_cast<TypeParam>(1.2),
            static_cast<TypeParam>(3.3),
            static_cast<TypeParam>(0.32),
            static_cast<TypeParam>(10.4)
        );

        constexpr Vec4 res1 (
            static_cast<TypeParam>(0.109275125f),
            static_cast<TypeParam>(0.300506569437f),
            static_cast<TypeParam>(0.0291400309757f),
            static_cast<TypeParam>(0.947051006709f)
        );

        pbrlib::testing::equality(res1, normalize(v1));
    }
}

TYPED_TEST(Vec4Tests, ZeroVectorNormalization)
{
    Vec4 v (
        static_cast<TypeParam>(0),
        static_cast<TypeParam>(0),
        static_cast<TypeParam>(0),
        static_cast<TypeParam>(0)
    );

    EXPECT_THROW({
        v.normalize();
    }, pbrlib::exception::MathError);

    EXPECT_THROW({
        [[maybe_unuse]] const auto res = normalize(v);
    }, pbrlib::exception::MathError);
}

TYPED_TEST(Vec4Tests, Lerp)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec4 result (static_cast<TypeParam>(0.75));
        pbrlib::testing::equality(result, lerp (
            Vec4(static_cast<TypeParam>(0)),
            Vec4(static_cast<TypeParam>(1)),
            static_cast<TypeParam>(0.75))
        );
    }
}
