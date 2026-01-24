#include "testing_types.hpp"

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/lerp.hpp>

using namespace pbrlib::math;

template <typename T>
class Vec2Tests
    : public ::testing::Test
{ };

TYPED_TEST_SUITE(Vec2Tests, MathTestingTypes);

TYPED_TEST(Vec2Tests, EqualAndNotEqual)
{
    constexpr Vec2 v1 (static_cast<TypeParam>(1));
    constexpr Vec2 v2 (static_cast<TypeParam>(1));
    constexpr Vec2 v3 (static_cast<TypeParam>(2));

    pbrlib::testing::equality(v1, v2);
    pbrlib::testing::notEquality(v1, v3);
}

TYPED_TEST(Vec2Tests, AdditionAndSubtraction)
{
    Vec2             v1      (static_cast<TypeParam>(2), static_cast<TypeParam>(3));
    constexpr Vec2   v2      (static_cast<TypeParam>(3), static_cast<TypeParam>(4));
    constexpr Vec2   res1    (static_cast<TypeParam>(5), static_cast<TypeParam>(7));
    Vec2             res2    (static_cast<TypeParam>(2), static_cast<TypeParam>(3));

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

TYPED_TEST(Vec2Tests, Multiplication)
{
    Vec2             v      (static_cast<TypeParam>(2), static_cast<TypeParam>(3));
    constexpr Vec2   res    (static_cast<TypeParam>(4), static_cast<TypeParam>(6));

    constexpr auto s = static_cast<TypeParam>(2);

    pbrlib::testing::equality(res, v * s);
    pbrlib::testing::equality(res, s * v);

    v *= s;

    pbrlib::testing::equality(res, v);
}

TYPED_TEST(Vec2Tests, AccessToElement)
{
    constexpr Vec2 v (static_cast<TypeParam>(4), static_cast<TypeParam>(123));

    pbrlib::testing::equality(static_cast<TypeParam>(4), v.x);
    pbrlib::testing::equality(static_cast<TypeParam>(123), v.y);

    pbrlib::testing::equality(static_cast<TypeParam>(4), v[0]);
    pbrlib::testing::equality(static_cast<TypeParam>(123), v[1]);
}

TYPED_TEST(Vec2Tests, Length)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec2 v1 (static_cast<TypeParam>(3434.323), static_cast<TypeParam>(121.5454));
        constexpr Vec2 v2 (static_cast<TypeParam>(0.132323), static_cast<TypeParam>(76.43400));

        pbrlib::testing::equality(static_cast<TypeParam>(3436.4731560991654), v1.length());
        pbrlib::testing::equality(static_cast<TypeParam>(76.43411453), v2.length());
    }
}

TYPED_TEST(Vec2Tests, Normalize)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec2 v1 (static_cast<TypeParam>(7643.8738), static_cast<TypeParam>(768.434));
        constexpr Vec2 v2 (static_cast<TypeParam>(0.1323230), static_cast<TypeParam>(76.4340));

        constexpr Vec2 r1 (static_cast<TypeParam>(0.9949848650), static_cast<TypeParam>(0.100025222));
        constexpr Vec2 r2 (static_cast<TypeParam>(0.0017312034), static_cast<TypeParam>(0.999998509));

        pbrlib::testing::equality(r1, normalize(v1));
        pbrlib::testing::equality(r2, normalize(v2));
    }
}

TYPED_TEST(Vec2Tests, ZeroVectorNormalization)
{
    Vec2 v (static_cast<TypeParam>(0), static_cast<TypeParam>(0));

    EXPECT_THROW({
        v.normalize();
    }, pbrlib::exception::MathError);

    EXPECT_THROW({
        [[maybe_unused]] const auto res = normalize(v);
    }, pbrlib::exception::MathError);
}

TYPED_TEST(Vec2Tests, Lerp)
{
    if constexpr (std::is_floating_point<TypeParam>::value)
    {
        constexpr Vec2 result (static_cast<TypeParam>(0.75f));
        pbrlib::testing::equality(result, lerp (
            Vec2(static_cast<TypeParam>(0)),
            Vec2(static_cast<TypeParam>(1)),
            static_cast<TypeParam>(0.75))
        );
    }
}
