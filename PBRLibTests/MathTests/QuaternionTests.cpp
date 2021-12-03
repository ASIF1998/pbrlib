//
//  QuaternionTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 16/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include "../../src/math/quaternion.hpp"
#include "../../src/math/vec3.hpp"

using namespace pbrlib::math;

TEST(MathQuaternion, Constructor)
{
    constexpr Vec3<float>   v (12.32f, 323.43f, 21.21f);
    constexpr float         w (22.22f);

    constexpr Quaternion q1;
    constexpr Quaternion q2 (1.0f, 2.2f, 4.255f, 123.321f);
    constexpr Quaternion q3 (v, w);

    pbrlib::testing::utils::equality(Vec3<float>(0.0f), q1.v);
    pbrlib::testing::utils::equality(1.0f, q1.w);

    pbrlib::testing::utils::equality(Vec3<float>(1.00000f, 2.20000f, 4.25500f), q2.v);
    pbrlib::testing::utils::equality(123.321f, q2.w);

    pbrlib::testing::utils::equality(v, q3.v);
    pbrlib::testing::utils::equality(w, q3.w);
}

TEST(MathQuaternion, EqualAndNotEqual)
{
    constexpr Quaternion    q1 (1.0f, 2.2f, 4.255f, 123.321f);
    Quaternion              q2 (1.0f, 2.2f, 4.255f, 123.321f);

    pbrlib::testing::utils::thisTrue(q1 == q2);

    q2.v.y += 59.34f;

    pbrlib::testing::utils::thisTrue(q1 != q2);
}

TEST(MathQuaternion, AdditionAndSubtraction)
{
    constexpr Quaternion res (2.0f, 6.0f, 8.0f, 10.0f);

    constexpr Quaternion    q1 (1.0f, 3.0f, 4.0f, 5.0f);
    Quaternion              q2 (1.0f, 3.0f, 4.0f, 5.0f);

    pbrlib::testing::utils::equality(res, q1 + q2);

    q2 += q1;

    pbrlib::testing::utils::equality(res, q2);
    pbrlib::testing::utils::equality(q1, q2 - q1);

    q2 -= q1;

    pbrlib::testing::utils::equality(q1, q2);
}

TEST(MathQuaternion, ScalarMultiplicationAndDivision)
{
    constexpr Quaternion res1 (3.0f, 9.0f, 12.0f, 15.0f);
    constexpr Quaternion res2 (1.0f, 3.0f, 4.00f, 5.00f);

    Quaternion  q (1.0f, 3.0f, 4.0f, 5.0f);
    float       s (3.0f);

    pbrlib::testing::utils::equality(res1, q * s);

    q *= s;

    pbrlib::testing::utils::equality(res1, q);
    pbrlib::testing::utils::equality(res2, q / s);

    q /= s;

    pbrlib::testing::utils::equality(res2, q);
}

TEST(MathQuaternion, QuaternionMultiplication)
{
    constexpr Quaternion res (6.0f, 8.0, 10.0f, -49.0f);

    constexpr Quaternion    q1 (3.0f, 4.0f, 5.0f, 1.0f);
    Quaternion              q2 (3.0f, 4.0f, 5.0f, 1.0f);
    
    pbrlib::testing::utils::equality(res, q1 * q2);

    q2 *= q1;

    pbrlib::testing::utils::equality(res, q2);
}

TEST(MathQuaternion, AccessToElement)
{
    constexpr Quaternion q (1.0f, 2.2f, 4.255f, 123.321f);

    pbrlib::testing::utils::equality(1.00000f, q.v.x);
    pbrlib::testing::utils::equality(2.20000f, q.v.y);
    pbrlib::testing::utils::equality(4.25500f, q.v.z);
    pbrlib::testing::utils::equality(123.321f, q.w);

    pbrlib::testing::utils::equality(1.00000f, q[0]);
    pbrlib::testing::utils::equality(2.20000f, q[1]);
    pbrlib::testing::utils::equality(4.25500f, q[2]);
    pbrlib::testing::utils::equality(123.321f, q[3]);
}

TEST(MathQuaternion, LenthAndLenthSquared)
{
    constexpr Quaternion q (3.0f, 4.0f, 5.0f, 1.0f);

    pbrlib::testing::utils::equality(51.0f, q.lengthSquared());
    pbrlib::testing::utils::equality(7.14142847f, q.length());
}

TEST(MathQuaternion, Normalize)
{
    constexpr Quaternion res (0.420084059f, 0.560112059f, 0.700140058f, 0.140028015f);

    Quaternion q (3.0f, 4.0f, 5.0f, 1.0f);

    pbrlib::testing::utils::equality(res, normalize(q));
    
    q.normalize();

    pbrlib::testing::utils::equality(res, q);
}

TEST(MathQuaternion, Inverse)
{
    constexpr Quaternion res (
        -0.0588235334f, 
        -0.0784313753f, 
        -0.0980392172f, 
        0.01960784380f
    );

    Quaternion q (3.0f, 4.0f, 5.0f, 1.0f);

    pbrlib::testing::utils::equality(res, inverse(q));

    q.inverse();

    pbrlib::testing::utils::equality(res, q);
}

TEST(MathQuaternion, StaticCreateQuaternionMethods)
{
    Quaternion q1 = Quaternion::i();
    Quaternion q2 = Quaternion::j();
    Quaternion q3 = Quaternion::k();
    Quaternion q4 = Quaternion::identity();
    Quaternion q5 = Quaternion::zerro();

    pbrlib::testing::utils::equality(Quaternion(1.0f, 0.0f, 0.0f, 0.0f), q1);
    pbrlib::testing::utils::equality(Quaternion(0.0f, 1.0f, 0.0f, 0.0f), q2);
    pbrlib::testing::utils::equality(Quaternion(0.0f, 0.0f, 1.0f, 0.0f), q3);
    pbrlib::testing::utils::equality(Quaternion(0.0f, 0.0f, 0.0f, 1.0f), q4);
    pbrlib::testing::utils::equality(Quaternion(0.0f, 0.0f, 0.0f, 0.0f), q5);
}

TEST(MathQuaternion, Lerp)
{
    constexpr Quaternion res (9.0f, 12.0f, 15.0f, 3.0f);

    constexpr Quaternion q1 (3.0f, 4.0f, 5.0f, 1.0f);
    constexpr Quaternion q2 (15.0f, 20.0f, 25.0f, 5.0f);

    pbrlib::testing::utils::equality(lerp(0.5f, q1, q2), res);
}

TEST(MathQuaternion, Slerp)
{
    constexpr Quaternion res (0.381873041f, 0.27716592f, 0.381873041f, 0.461943209f);

    constexpr Quaternion q1 (0.12f, 0.300f, 0.4000f, 0.500f);
    constexpr Quaternion q2 (-0.5f, -0.15f, -0.220f, -0.25f);

    pbrlib::testing::utils::equality(slerp(0.5f, q1, q2), res);
}

TEST(MathQuaternion, ToTransform)
{
    constexpr Matrix4x4<float> res (
        1.0f, 0.00f, 0.00f, 0.0f,
        0.0f, -1.0f, 0.00f, 0.0f,
        0.0f, 0.00f, -1.0f, 0.0f,
        0.0f, 0.00f, 0.00f, 1.0f
    );

    constexpr Quaternion q (1.0f, 0.0f, 0.0f, 0.0f);

    pbrlib::testing::utils::equality(q.toTransform().getMatrix(), res);
}
