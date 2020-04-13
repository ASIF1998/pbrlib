//
//  Vec2Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 12/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/math/vec2.hpp"

using namespace testing;
using namespace pbrlib::math;

TEST(MathVec2, EqualAndNotEqual)
{
    Vec2<float> v1 (1.0f);
    Vec2<float> v2 (1.0f);
    
    Vec2<int> v3 (1, 3);
    Vec2<int> v4 (1, 3);

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

TEST(MathVec2, AdditionAndSubtraction)
{
    Vec2<float>             v1  (2.0f, 3.0f);
    constexpr Vec2<float>   v2  (3.5f, 4.56f);
    constexpr Vec2<float>   res1(5.5f, 7.56f);
    Vec2<float>             res2(2.0f, 3.0f);

    EXPECT_EQ(res1, v1 + v2);

    v1 += v2;

    EXPECT_EQ(res1, v1);

    v1 -= v2;

    EXPECT_EQ(res2, v1);

    res2 -= v2;

    EXPECT_EQ(res2, v1 - v2);
}

TEST(MathVec2, Multiplication)
{
    Vec2<float>             v1  (2.0f, 3.0f);
    constexpr Vec2<float>   res (4.0f, 6.0f);
    constexpr float         s   (2.0f);

    EXPECT_EQ(res, v1 * s);

    v1 *= s;

    EXPECT_EQ(res, v1);
}

TEST(MathVec2, AccessToElement)
{
    constexpr Vec2<int> v (4, 1232);

    EXPECT_EQ(4, v.x);
    EXPECT_EQ(1232, v.y);

    EXPECT_EQ(4, v[0]);
    EXPECT_EQ(1232, v[1]);
}

TEST(MathVec2, Length)
{
    constexpr Vec2<float> v1 (3434.323f, 121.5454f);
    constexpr Vec2<float> v2 (0.132323f, 76.434f);

    EXPECT_EQ(3436.473156098f, v1.length());
    EXPECT_EQ(76.43411453f, v2.length());
}

TEST(MathVec2, Normalize)
{
    constexpr Vec2<float> v1 (7643.8738f, 768.434f);
    constexpr Vec2<float> v2 (0.132323f, 76.434f);

    constexpr Vec2<float> r1 (0.994984865f, 0.100025222f);
    constexpr Vec2<float> r2 (0.0017312034f, 0.999998509f);
    
    EXPECT_TRUE(r1 == normalize(v1));
    EXPECT_TRUE(r2 == normalize(v2));
}
