//
//  Vec4Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 13/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/math/vec4.hpp"

using namespace testing;
using namespace pbrlib::math;

TEST(MathVec4, EqualAndNotEqual)
{
    Vec4<float> v1 (3.5f, 23.43f, 343.54f, 233.5f);
    Vec4<float> v2 (3.5f, 23.43f, 343.54f, 233.5f);

    Vec4<int> v3 (1, 2, 3, 5);
    Vec4<int> v4 (1, 2, 3, 5);

    EXPECT_TRUE(v1 == v2);
    EXPECT_TRUE(v3 == v4);

    v1[0] = 32.43434f;
    v3[0] = 234234;

    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v3 != v4);
}

TEST(MathVec4, AdditionAndSubtraction)
{
    constexpr Vec4<float> res1 (2.0f, 5.0f, 2.4f, 7.0f);
    constexpr Vec4<float> res2 (0.0f, 0.0f, 0.0f, 0.0f);

    constexpr Vec4<int> res3 (2, 4, 6, 8);
    constexpr Vec4<int> res4 (0, 0, 0, 0);

    Vec4<float> v1 (1.0f, 2.5f, 1.2f, 3.5f);
    Vec4<float> v2 (1.0f, 2.5f, 1.2f, 3.5f);

    Vec4<int> v3 (1, 2, 3, 4);
    Vec4<int> v4 (1, 2, 3, 4);

    EXPECT_EQ(res1, v1 + v2);
    EXPECT_EQ(res3, v3 + v4);
    EXPECT_EQ(res2, v1 - v2);
    EXPECT_EQ(res4, v3 - v4);

    v1 += v2;
    v3 += v4;

    EXPECT_EQ(res1, v1);
    EXPECT_EQ(res3, v3);

    v1 -= v2;
    v1 -= v2;

    v3 -= v4;
    v3 -= v4;

    EXPECT_EQ(res4, v3);
}

TEST(MathVec4, Multiplication)
{
    constexpr Vec4<float>   res1 (11.0f, 9.0f, 24.8f, 7.2f);
    constexpr Vec4<int>     res2 (6, 9, 12 ,15);

    Vec4<float> v1 (5.5f, 4.5f, 12.4f, 3.6f);
    Vec4<int>   v2 (2, 3, 4, 5);

    float   s1  = 2.0f;
    int     s2  = 3;

    EXPECT_EQ(res1, v1 * s1);
    EXPECT_EQ(res2, v2 * s2);

    EXPECT_EQ(v1.lengthSquared(), dot(v1, v1));
    EXPECT_EQ(v2.lengthSquared(), dot(v2, v2));

    v1 *= s1;
    v2 *= s2;

    EXPECT_EQ(res1, v1);
    EXPECT_EQ(res2, v2);

    EXPECT_EQ(v1.lengthSquared(), dot(v1, v1));
    EXPECT_EQ(v2.lengthSquared(), dot(v2, v2));
}

TEST(MathVec4, AccessToElement)
{
    constexpr Vec4<float>   v1 (2.2f, 4.3f, 54.4f, 0.0005f);
    constexpr Vec4<int>     v2 (1, 2, 3, 7);

    EXPECT_EQ(2.2f, v1[0]);
    EXPECT_EQ(4.3f, v1[1]);
    EXPECT_EQ(54.4f, v1[2]);
    EXPECT_EQ(0.0005f, v1[3]);

    EXPECT_EQ(2.2f, v1.x);
    EXPECT_EQ(4.3f, v1.y);
    EXPECT_EQ(54.4f, v1.z);
    EXPECT_EQ(0.0005f, v1.w);

    EXPECT_EQ(1, v2[0]);
    EXPECT_EQ(2, v2[1]);
    EXPECT_EQ(3, v2[2]);
    EXPECT_EQ(7, v2[3]);

    EXPECT_EQ(1, v2.x);
    EXPECT_EQ(2, v2.y);
    EXPECT_EQ(3, v2.z);
    EXPECT_EQ(7, v2.w);
}

TEST(MathVec4, Length)
{
    constexpr Vec4<float>   v1 (1.2f, 3.3f, 0.32f, 10.4f);
    constexpr Vec4<int>     v2 (1, 2, 3, 4);

    EXPECT_EQ(10.9814568f, v1.length());
    EXPECT_EQ(5, v2.length());
}

TEST(MathVec4, Normalize)
{
    constexpr Vec4<float>   v1 (1.2f, 3.3f, 0.32f, 10.4f);
    constexpr Vec4<int>     v2 (1, 2, 3, 4);

    constexpr Vec4<float>   res1 (0.109275125f, 0.300506569437f, 0.0291400309757f, 0.947051006709f);
    constexpr Vec4<int>     res2 (0, 0, 0, 0);

    EXPECT_TRUE(res1 == normalize(v1));
    EXPECT_TRUE(res2 == normalize(v2));
}
