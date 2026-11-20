#include "testing_types.hpp"

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/lerp.hpp>

using namespace pbrlib::math;

template <typename T>
class Vec3Tests
    : public ::testing::Test
{ };

TYPED_TEST_SUITE(Vec3Tests, MathTestingTypes);

TYPED_TEST(Vec3Tests, EqualAndNotEqual)
{
    constexpr Vec3 v1 (static_cast<TypeParam>(1));
    constexpr Vec3 v2 (static_cast<TypeParam>(1));
    constexpr Vec3 v3 (static_cast<TypeParam>(2));

    pbrlib::testing::equality(v1, v2);
    pbrlib::testing::notEquality(v1, v3);
}

TYPED_TEST(Vec3Tests, AdditionAndSubtraction)
{
    Vec3             v1      (static_cast<TypeParam>(2), static_cast<TypeParam>(3));
    constexpr Vec3   v2      (static_cast<TypeParam>(3), static_cast<TypeParam>(4));
    constexpr Vec3   res1    (static_cast<TypeParam>(5), static_cast<TypeParam>(7));
    Vec3             res2    (static_cast<TypeParam>(2), static_cast<TypeParam>(3));

    auto res3 = v1 + v2;

    pbrlib::testing::equality(res1, res3);

    v1 += v2;

    pbrlib::testing::equality(res1, v1);

    v1 -= v2;

    pbrlib::testing::equality(res2, v1);

    res2 -= v2;
    res3 =  v1 - v2;

    pbrlib::testing::equality(res2, res3);
}

TYPED_TEST(Vec3Tests, Multiplication)
{
    Vec3 v (
        static_cast<TypeParam>(2),
        static_cast<TypeParam>(3),
        static_cast<TypeParam>(6)
    );

    constexpr Vec3 res (
        static_cast<TypeParam>(4),
        static_cast<TypeParam>(6),
        static_cast<TypeParam>(12)
    );

    constexpr auto s = static_cast<TypeParam>(2);

    pbrlib::testing::equality(res, v * s);
    pbrlib::testing::equality(res, s * v);

    v *= s;

    pbrlib::testing::equality(res, v);
}

TYPED_TEST(Vec3Tests, AccessToElement)
{
    Vec3 v (static_cast<TypeParam>(1), static_cast<TypeParam>(2), static_cast<TypeParam>(3));

    pbrlib::testing::equality(static_cast<TypeParam>(1), v[0]);
    pbrlib::testing::equality(static_cast<TypeParam>(2), v[1]);
    pbrlib::testing::equality(static_cast<TypeParam>(3), v[2]);

    pbrlib::testing::equality(static_cast<TypeParam>(1), v.x);
    pbrlib::testing::equality(static_cast<TypeParam>(2), v.y);
    pbrlib::testing::equality(static_cast<TypeParam>(3), v.z);
}

TYPED_TEST(Vec3Tests, Length)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec3 v    (static_cast<TypeParam>(1.2), static_cast<TypeParam>(32.4), static_cast<TypeParam>(12.4));
        constexpr auto lr   = static_cast<TypeParam>(34.7125359);

        pbrlib::testing::equality(lr, v.length());
    }
}

TYPED_TEST(Vec3Tests, Normalize)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec3 r1 (
            static_cast<TypeParam>(0.99497598400),
            static_cast<TypeParam>(0.100024336737),
            static_cast<TypeParam>(0.00422390176269)
        );

        constexpr Vec3 r2 (
            static_cast<TypeParam>(0.00131073419),
            static_cast<TypeParam>(0.757122039000),
            static_cast<TypeParam>(0.65327209200000)
        );

        constexpr Vec3 v1 (
            static_cast<TypeParam>(7643.8738),
            static_cast<TypeParam>(768.434),
            static_cast<TypeParam>(32.45)
        );

        constexpr Vec3 v2 (
            static_cast<TypeParam>(0.1323230),
            static_cast<TypeParam>(76.4340),
            static_cast<TypeParam>(65.95)
        );

        pbrlib::testing::equality(r1, normalize(v1));
        pbrlib::testing::equality(r2, normalize(v2));
    }
}

TYPED_TEST(Vec3Tests, ZeroVectorNormalization)
{
    Vec3 v (
        static_cast<TypeParam>(0),
        static_cast<TypeParam>(0),
        static_cast<TypeParam>(0)
    );

    EXPECT_THROW({
        v.normalize();
    }, pbrlib::exception::MathError);

    EXPECT_THROW({
        [[maybe_unused]] const auto res = normalize(v);
    }, pbrlib::exception::MathError);
}

TYPED_TEST(Vec3Tests, Lerp)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec3 result (static_cast<TypeParam>(0.75));
        pbrlib::testing::equality(result, lerp (
            Vec3(static_cast<TypeParam>(0)),
            Vec3(static_cast<TypeParam>(1)),
            static_cast<TypeParam>(0.75))
        );
    }
}
