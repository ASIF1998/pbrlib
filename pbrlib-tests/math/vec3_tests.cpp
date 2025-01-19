#include "../utils/assertions.hpp"

#include <pbrlib/math/vec3.hpp>

using namespace pbrlib::math;

TEST(Vec3Tests, EqualAndNotEqual)
{
    vec3 v1 (1.0f);
    vec3 v2 (1.0f);
    
    ivec3 v3 (1, 3, 23);
    ivec3 v4 (1, 3, 23);

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

TEST(Vec3Tests, AdditionAndSubtraction)
{
    vec3             v1  (2.0f, 3.00f, 3.5f);
    constexpr vec3   v2  (3.5f, 4.56f, 3.5f);
    constexpr vec3   res1(5.5f, 7.56f, 7.0f);
    vec3             res2(2.0f, 3.00f, 3.5f);

    vec3 res3 = v1 + v2;

    pbrlib::testing::equality(res1, res3);

    v1 += v2;

    pbrlib::testing::equality(res1, v1);

    v1 -= v2;

    pbrlib::testing::equality(res2, v1);

    res2 -= v2;
    res3 =  v1 - v2;

    pbrlib::testing::equality(res2, res3);
}

TEST(Vec3Tests, Multiplication)
{
    constexpr vec3   sres    (2.400000000f, 64.8000000f, 24.80000000f);
    constexpr vec3   cres    (0.718162537f, 40.9439163f, -107.052002f);
    constexpr float         dres    (130.6861720f);

    vec3 v1 (1.20f, 32.4f, 12.400f);
    vec3 v2 (3.43f, 3.40f, 1.3234f);
    
    pbrlib::testing::equality(sres, v1 * 2.0f);
    pbrlib::testing::equality(cres, cross(v1, v2));
    pbrlib::testing::equality(v1.lengthSquared(), dot(v1, v1));
    pbrlib::testing::equality(dres, dot(v1, v2));
}

TEST(Vec3Tests, AccessToElement)
{
    ivec3 v (1, 2, 3);

    pbrlib::testing::equality(1, v[0]);
    pbrlib::testing::equality(2, v[1]);
    pbrlib::testing::equality(3, v[2]);

    pbrlib::testing::equality(1, v.x);
    pbrlib::testing::equality(2, v.y);
    pbrlib::testing::equality(3, v.z);
}

TEST(Vec3Tests, Length)
{
    constexpr vec3   v   (1.20000000f, 32.4f, 12.4f);
    constexpr float         lr  (34.7125359f);

    pbrlib::testing::equality(lr, v.length());
}

TEST(Vec3Tests, Normalize)
{
    constexpr vec3 r1 (0.99497598400f, 0.100024336737f, 0.00422390176269f);
    constexpr vec3 r2 (0.00131073419f, 0.757122039000f, 0.65327209200000f);

    constexpr vec3 v1 (7643.8738f, 768.434f, 32.45f);
    constexpr vec3 v2 (0.1323230f, 76.4340f, 65.95f);

    pbrlib::testing::equality(r1, normalize(v1));
    pbrlib::testing::equality(r2, normalize(v2));
}
