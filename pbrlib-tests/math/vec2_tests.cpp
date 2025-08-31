#include "../utils.hpp"

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/lerp.hpp>

using namespace pbrlib::math;

TEST(Vec2Tests, EqualAndNotEqual)
{
    vec2 v1 (1.0f);
    vec2 v2 (1.0f);
    
    ivec2 v3 (1, 3);
    ivec2 v4 (1, 3);

    pbrlib::testing::equality(v1, v2);
    pbrlib::testing::equality(v3, v4);

    pbrlib::testing::thisTrue(v1 == v2);
    pbrlib::testing::thisTrue(v3 == v4);

    v1[0] = 4.0f;
    v3[0] = 4;

    pbrlib::testing::notEquality(v1, v2);
    pbrlib::testing::notEquality(v3, v4);

    pbrlib::testing::thisTrue(v1 != v2);
    pbrlib::testing::thisTrue(v3 != v4);
}

TEST(Vec2Tests, AdditionAndSubtraction)
{
    vec2             v1      (2.0f, 3.00f);
    constexpr vec2   v2      (3.5f, 4.56f);
    constexpr vec2   res1    (5.5f, 7.56f);
    vec2             res2    (2.0f, 3.00f);

    vec2 res3 = v1 + v2;

    pbrlib::testing::equality(res1, res3);

    v1 += v2;

    pbrlib::testing::equality(res1, v1);

    v1 -= v2;

    pbrlib::testing::equality(res2, v1);

    res2 -= v2;
    res3 =  v1 - v2;

    pbrlib::testing::equality(res2, res3);
}

TEST(Vec2Tests, Multiplication)
{
    vec2             v       (2.0f, 3.0f);
    constexpr vec2   res     (4.0f, 6.0f);
    constexpr float         s       (2.0f);

    pbrlib::testing::equality(res, v * s);

    v *= s;

    pbrlib::testing::equality(res, v);
}

TEST(Vec2Tests, AccessToElement)
{
    constexpr ivec2 v (4, 1232);

    pbrlib::testing::equality(4, v.x);
    pbrlib::testing::equality(1232, v.y);

    pbrlib::testing::equality(4, v[0]);
    pbrlib::testing::equality(1232, v[1]);
}

TEST(Vec2Tests, Length)
{
    constexpr vec2 v1 (3434.323f, 121.5454f);
    constexpr vec2 v2 (0.132323f, 76.43400f);

    pbrlib::testing::equality(3436.473156098f, v1.length());
    pbrlib::testing::equality(76.43411453f, v2.length());
}

TEST(Vec2Tests, Normalize)
{
    constexpr vec2 v1 (7643.8738f, 768.434f);
    constexpr vec2 v2 (0.1323230f, 76.4340f);

    constexpr vec2 r1 (0.9949848650f, 0.100025222f);
    constexpr vec2 r2 (0.0017312034f, 0.999998509f);
    
    pbrlib::testing::equality(r1, normalize(v1));
    pbrlib::testing::equality(r2, normalize(v2));
}

TEST(Vec2Tests, NormalizeEdgeCase)
{
    vec2 v (0, 0);

    EXPECT_THROW({
        v.normalize();
    }, pbrlib::exception::MathError);
    
    EXPECT_THROW({
        [[maybe_unuse]] const auto res = normalize(v);
    }, pbrlib::exception::MathError);
}

TEST(Vec2Tests, Lerp)
{
    constexpr vec2 result (0.75f);
    pbrlib::testing::equality(result, lerp(vec2(0.0f), vec2(1.0f), 0.75f));
}