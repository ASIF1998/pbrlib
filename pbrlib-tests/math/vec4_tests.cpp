#include "../utils.hpp"

#include <pbrlib/math/vec4.hpp>

using namespace pbrlib::math;

TEST(Vec4Tests, EqualAndNotEqual)
{
    vec4 v1 (3.5f, 23.43f, 343.54f, 233.5f);
    vec4 v2 (3.5f, 23.43f, 343.54f, 233.5f);

    ivec4 v3 (1, 2, 3, 5);
    ivec4 v4 (1, 2, 3, 5);

    pbrlib::testing::thisTrue(v1 == v2);
    pbrlib::testing::thisTrue(v3 == v4);

    v1[0] = 32.43434f;
    v3[0] = 234234;

    pbrlib::testing::thisTrue(v1 != v2);
    pbrlib::testing::thisTrue(v3 != v4);
}

TEST(Vec4Tests, AdditionAndSubtraction)
{
    constexpr vec4 res1 (2.0f, 5.0f, 2.4f, 7.0f);
    constexpr vec4 res2 (0.0f, 0.0f, 0.0f, 0.0f);

    constexpr ivec4 res3 (2, 4, 6, 8);
    constexpr ivec4 res4 (0, 0, 0, 0);

    vec4 v1 (1.0f, 2.5f, 1.2f, 3.5f);
    vec4 v2 (1.0f, 2.5f, 1.2f, 3.5f);

    ivec4 v3 (1, 2, 3, 4);
    ivec4 v4 (1, 2, 3, 4);

    vec4 res5 = v1 + v2;

    pbrlib::testing::equality(res1, res5);

    res5 = v1 - v2;

    pbrlib::testing::equality(res2, res5);

    pbrlib::testing::equality(res3, v3 + v4);
    pbrlib::testing::equality(res4, v3 - v4);

    v1 += v2;
    v3 += v4;

    pbrlib::testing::equality(res1, v1);
    pbrlib::testing::equality(res3, v3);

    v3 -= v4;
    v3 -= v4;

    pbrlib::testing::equality(res4, v3);
}

TEST(Vec4Tests, Multiplication)
{
    constexpr vec4  res1 (11.0f, 9.0f, 24.8f, 7.2f);
    constexpr ivec4 res2 (6, 9, 12 ,15);

    vec4    v1 (5.5f, 4.5f, 12.4f, 3.6f);
    ivec4   v2 (2, 3, 4, 5);

    float   s1  = 2.0f;
    int     s2  = 3;

    vec4 res3 = v1 * s1;


    pbrlib::testing::equality(res1, res3);

    pbrlib::testing::equality(res2, v2 * s2);

    pbrlib::testing::equality(v1.lengthSquared(), dot(v1, v1));
    pbrlib::testing::equality(v2.lengthSquared(), dot(v2, v2));

    v1 *= s1;
    v2 *= s2;

    pbrlib::testing::equality(res1, v1);
    pbrlib::testing::equality(res2, v2);
    pbrlib::testing::equality(v1.lengthSquared(), dot(v1, v1));
    pbrlib::testing::equality(v2.lengthSquared(), dot(v2, v2));
}

TEST(Vec4Tests, AccessToElement)
{
    constexpr vec4  v1 (2.2f, 4.3f, 54.4f, 0.0005f);
    constexpr ivec4 v2 (1, 2, 3, 7);

    pbrlib::testing::equality(2.2000f, v1[0]);
    pbrlib::testing::equality(4.3000f, v1[1]);
    pbrlib::testing::equality(54.400f, v1[2]);
    pbrlib::testing::equality(0.0005f, v1[3]);

    pbrlib::testing::equality(2.2000f, v1.x);
    pbrlib::testing::equality(4.3000f, v1.y);
    pbrlib::testing::equality(54.400f, v1.z);
    pbrlib::testing::equality(0.0005f, v1.w);

    pbrlib::testing::equality(1, v2[0]);
    pbrlib::testing::equality(2, v2[1]);
    pbrlib::testing::equality(3, v2[2]);
    pbrlib::testing::equality(7, v2[3]);

    pbrlib::testing::equality(1, v2.x);
    pbrlib::testing::equality(2, v2.y);
    pbrlib::testing::equality(3, v2.z);
    pbrlib::testing::equality(7, v2.w);
}

TEST(Vec4Tests, Length)
{
    constexpr vec4  v1 (1.2f, 3.3f, 0.32f, 10.4f);
    constexpr ivec4 v2 (1, 2, 3, 4);

    pbrlib::testing::equality(10.9814568f, v1.length());
    pbrlib::testing::equality(5, v2.length());
}

TEST(Vec4Tests, Normalize)
{
    constexpr vec4  v1 (1.2f, 3.3f, 0.32f, 10.4f);
    constexpr ivec4 v2 (1, 2, 3, 4);

    constexpr vec4  res1 (0.109275125f, 0.300506569437f, 0.0291400309757f, 0.947051006709f);
    constexpr ivec4 res2 (0, 0, 0, 0);

    pbrlib::testing::equality(res1, normalize(v1));
    pbrlib::testing::equality(res2, normalize(v2));
}

TEST(Vec4Tests, NormalizeEdgeCase)
{
    vec4 v (0, 0, 0, 0);

    EXPECT_THROW({
        v.normalize();
    }, pbrlib::exception::MathError);
    
    EXPECT_THROW({
        [[maybe_unuse]] const auto res = normalize(v);
    }, pbrlib::exception::MathError);
}