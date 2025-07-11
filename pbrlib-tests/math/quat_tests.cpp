#include "../utils.hpp"

#include <pbrlib/math/quat.hpp>
#include <pbrlib/math/vec3.hpp>

TEST(QuatTests, Constructor)
{
    constexpr pbrlib::math::vec3 v (12.32f, 323.43f, 21.21f);

    constexpr float w = 22.22f;

    constexpr pbrlib::math::quat q1;
    constexpr pbrlib::math::quat q2 (1.0f, 2.2f, 4.255f, 123.321f);
    constexpr pbrlib::math::quat q3 (v, w);

    pbrlib::testing::equality(pbrlib::math::vec3(0.0f), q1.v);
    pbrlib::testing::equality(1.0f, q1.w);

    pbrlib::testing::equality(pbrlib::math::vec3(1.00000f, 2.20000f, 4.25500f), q2.v);
    pbrlib::testing::equality(123.321f, q2.w);

    pbrlib::testing::equality(v, q3.v);
    pbrlib::testing::equality(w, q3.w);
}

TEST(QuatTests, EqualAndNotEqual)
{
    constexpr pbrlib::math::quat    q1 (1.0f, 2.2f, 4.255f, 123.321f);
    pbrlib::math::quat              q2 (1.0f, 2.2f, 4.255f, 123.321f);

    pbrlib::testing::thisTrue(q1 == q2);

    q2.v.y += 59.34f;

    pbrlib::testing::thisTrue(q1 != q2);
}

TEST(QuatTests, AdditionAndSubtraction)
{
    constexpr pbrlib::math::quat res (2.0f, 6.0f, 8.0f, 10.0f);

    constexpr pbrlib::math::quat    q1 (1.0f, 3.0f, 4.0f, 5.0f);
    pbrlib::math::quat              q2 (1.0f, 3.0f, 4.0f, 5.0f);

    pbrlib::testing::equality(res, q1 + q2);

    q2 += q1;

    pbrlib::testing::equality(res, q2);
    pbrlib::testing::equality(q1, q2 - q1);

    q2 -= q1;

    pbrlib::testing::equality(q1, q2);
}

TEST(QuatTests, ScalarMultiplicationAndDivision)
{
    constexpr pbrlib::math::quat res1 (3.0f, 9.0f, 12.0f, 15.0f);
    constexpr pbrlib::math::quat res2 (1.0f, 3.0f, 4.00f, 5.00f);

    pbrlib::math::quat q (1.0f, 3.0f, 4.0f, 5.0f);

    float s = 3.0f;

    pbrlib::testing::equality(res1, q * s);

    q *= s;

    pbrlib::testing::equality(res1, q);
    pbrlib::testing::equality(res2, q / s);

    q /= s;

    pbrlib::testing::equality(res2, q);
}

TEST(QuatTests, quatMultiplication)
{
    constexpr pbrlib::math::quat res (6.0f, 8.0, 10.0f, -49.0f);

    constexpr pbrlib::math::quat    q1 (3.0f, 4.0f, 5.0f, 1.0f);
    pbrlib::math::quat              q2 (3.0f, 4.0f, 5.0f, 1.0f);
    
    pbrlib::testing::equality(res, q1 * q2);

    q2 *= q1;

    pbrlib::testing::equality(res, q2);
}

TEST(QuatTests, AccessToElement)
{
    constexpr pbrlib::math::quat q (1.0f, 2.2f, 4.255f, 123.321f);

    pbrlib::testing::equality(1.00000f, q.v.x);
    pbrlib::testing::equality(2.20000f, q.v.y);
    pbrlib::testing::equality(4.25500f, q.v.z);
    pbrlib::testing::equality(123.321f, q.w);

    pbrlib::testing::equality(1.00000f, q[0]);
    pbrlib::testing::equality(2.20000f, q[1]);
    pbrlib::testing::equality(4.25500f, q[2]);
    pbrlib::testing::equality(123.321f, q[3]);
}

TEST(QuatTests, LenthAndLenthSquared)
{
    constexpr pbrlib::math::quat q (3.0f, 4.0f, 5.0f, 1.0f);

    pbrlib::testing::equality(51.0f, q.lengthSquared());
    pbrlib::testing::equality(7.14142847f, q.length());
}

TEST(QuatTests, Normalize)
{
    constexpr pbrlib::math::quat res (0.420084059f, 0.560112059f, 0.700140058f, 0.140028015f);

    pbrlib::math::quat q (3.0f, 4.0f, 5.0f, 1.0f);

    pbrlib::testing::equality(res, pbrlib::math::normalize(q));
    
    q.normalize();

    pbrlib::testing::equality(res, q);
}

TEST(QuatTests, Inverse)
{
    constexpr pbrlib::math::quat res (
        -0.0588235334f, 
        -0.0784313753f, 
        -0.0980392172f, 
        0.01960784380f
    );

    pbrlib::math::quat q (3.0f, 4.0f, 5.0f, 1.0f);

    pbrlib::testing::equality(res, pbrlib::math::inverse(q));

    q.inverse();

    pbrlib::testing::equality(res, q);
}

TEST(QuatTests, StaticCreatequatMethods)
{
    const auto q1 = pbrlib::math::quat::i();
    const auto q2 = pbrlib::math::quat::j();
    const auto q3 = pbrlib::math::quat::k();
    const auto q4 = pbrlib::math::quat::identity();
    const auto q5 = pbrlib::math::quat::zerro();

    pbrlib::testing::equality(pbrlib::math::quat(1.0f, 0.0f, 0.0f, 0.0f), q1);
    pbrlib::testing::equality(pbrlib::math::quat(0.0f, 1.0f, 0.0f, 0.0f), q2);
    pbrlib::testing::equality(pbrlib::math::quat(0.0f, 0.0f, 1.0f, 0.0f), q3);
    pbrlib::testing::equality(pbrlib::math::quat(0.0f, 0.0f, 0.0f, 1.0f), q4);
    pbrlib::testing::equality(pbrlib::math::quat(0.0f, 0.0f, 0.0f, 0.0f), q5);
}

TEST(QuatTests, Lerp)
{
    constexpr pbrlib::math::quat res (9.0f, 12.0f, 15.0f, 3.0f);

    constexpr pbrlib::math::quat q1 (3.0f, 4.0f, 5.0f, 1.0f);
    constexpr pbrlib::math::quat q2 (15.0f, 20.0f, 25.0f, 5.0f);

    pbrlib::testing::equality(pbrlib::math::lerp(0.5f, q1, q2), res);
}

TEST(QuatTests, Slerp)
{
    constexpr pbrlib::math::quat res (0.381873041f, 0.27716592f, 0.381873041f, 0.461943209f);

    constexpr pbrlib::math::quat q1 (0.12f, 0.300f, 0.4000f, 0.500f);
    constexpr pbrlib::math::quat q2 (-0.5f, -0.15f, -0.220f, -0.25f);

    pbrlib::testing::equality(pbrlib::math::slerp(0.5f, q1, q2), res);
}

TEST(QuatTests, ToTransform)
{
    constexpr pbrlib::math::mat4 res (
        1.0f, 0.00f, 0.00f, 0.0f,
        0.0f, -1.0f, 0.00f, 0.0f,
        0.0f, 0.00f, -1.0f, 0.0f,
        0.0f, 0.00f, 0.00f, 1.0f
    );

    constexpr pbrlib::math::quat q (1.0f, 0.0f, 0.0f, 0.0f);

    pbrlib::testing::equality(q.toMatrix(), res);
}

TEST(QuatTests, NormalizeEdgeCase)
{
    pbrlib::math::quat q (0, 0, 0, 0);

    EXPECT_THROW({
        q.normalize();
    }, pbrlib::exception::MathError);
}

TEST(QuatTests, InverseEdgeCase)
{
    pbrlib::math::quat q (0, 0, 0, 0);

    EXPECT_THROW({
        q.inverse();
    }, pbrlib::exception::MathError);
}