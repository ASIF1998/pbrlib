//
//  Vec3Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 12/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/math/vec3.hpp"

using namespace testing;
using namespace pbrlib::math;

TEST(MathVec3, EqualAndNotEqual)
{
    Vec3<float> v1 (1.0f);
    Vec3<float> v2 (1.0f);
    
    Vec3<int> v3 (1, 3, 23);
    Vec3<int> v4 (1, 3, 23);

    EXPECT_EQ(v1, v2);
    EXPECT_EQ(v3, v4);

    EXPECT_TRUE(v1 == v2);
    EXPECT_TRUE(v3 == v4);

    v1[0] = 4.0f;
    v3[0] = 4;

    EXPECT_NE(v1, v2);
    EXPECT_NE(v3, v4);

    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v3 != v4);
}

TEST(MathVec3, AdditionAndSubtraction)
{
    Vec3<float>             v1  (2.0f, 3.00f, 3.5f);
    constexpr Vec3<float>   v2  (3.5f, 4.56f, 3.5f);
    constexpr Vec3<float>   res1(5.5f, 7.56f, 7.0f);
    Vec3<float>             res2(2.0f, 3.00f, 3.5f);

    EXPECT_EQ(res1, v1 + v2);

    v1 += v2;

    EXPECT_EQ(res1, v1);

    v1 -= v2;

    EXPECT_EQ(res2, v1);

    res2 -= v2;

    EXPECT_EQ(res2, v1 - v2);
}

TEST(MathVec3, Multiplication)
{
    constexpr Vec3<float>   sres    (2.400000000f, 64.8000000f, 24.80000000f);
    constexpr Vec3<float>   cres    (0.718162537f, 40.9439163f, -107.052002f);
    constexpr float         dres    (130.6861720f);

    Vec3<float> v1 (1.20f, 32.4f, 12.400f);
    Vec3<float> v2 (3.43f, 3.40f, 1.3234f);
    
    EXPECT_TRUE(sres == (v1 * 2.0f));
    EXPECT_TRUE(cres == cross(v1, v2));
    EXPECT_EQ(v1.lengthSquared(), dot(v1, v1));
    EXPECT_EQ(dres, dot(v1, v2));
}

TEST(MathVec3, AccessToElement)
{
    Vec3<int> v (1, 2, 3);

    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);

    EXPECT_EQ(1, v.x);
    EXPECT_EQ(2, v.y);
    EXPECT_EQ(3, v.z);
}

TEST(MathVec3, Length)
{
    constexpr Vec3<float>   v   (1.20000000f, 32.4f, 12.4f);
    constexpr float         lr  (34.7125359f);

    EXPECT_EQ(lr, v.length());
}

TEST(MathVec3, Normalize)
{
    constexpr Vec3<float> r1 (0.99497598400f, 0.100024336737f, 0.00422390176269f);
    constexpr Vec3<float> r2 (0.00131073419f, 0.757122039000f, 0.65327209200000f);

    constexpr Vec3<float> v1 (7643.8738f, 768.434f, 32.45f);
    constexpr Vec3<float> v2 (0.1323230f, 76.4340f, 65.95f);

    EXPECT_TRUE(r1 == normalize(v1));
    EXPECT_TRUE(r2 == normalize(v2));
}
