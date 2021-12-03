//
//  Vec2Tests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 12/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include "../../src/math/vec2.hpp"

using namespace pbrlib::math;

TEST(MathVec2, EqualAndNotEqual)
{
    Vec2<float> v1 (1.0f);
    Vec2<float> v2 (1.0f);
    
    Vec2<int> v3 (1, 3);
    Vec2<int> v4 (1, 3);

    pbrlib::testing::utils::equality(v1, v2);
    pbrlib::testing::utils::equality(v3, v4);

    pbrlib::testing::utils::thisTrue(v1 == v2);
    pbrlib::testing::utils::thisTrue(v3 == v4);

    v1[0] = 4.0f;
    v3[0] = 4;

    pbrlib::testing::utils::notEquality(v1, v2);
    pbrlib::testing::utils::notEquality(v3, v4);

    pbrlib::testing::utils::thisTrue(v1 != v2);
    pbrlib::testing::utils::thisTrue(v3 != v4);
}

TEST(MathVec2, AdditionAndSubtraction)
{
    Vec2<float>             v1      (2.0f, 3.00f);
    constexpr Vec2<float>   v2      (3.5f, 4.56f);
    constexpr Vec2<float>   res1    (5.5f, 7.56f);
    Vec2<float>             res2    (2.0f, 3.00f);

    Vec2<float> res3 = v1 + v2;

    pbrlib::testing::utils::equality(res1, res3);

    v1 += v2;

    pbrlib::testing::utils::equality(res1, v1);

    v1 -= v2;

    pbrlib::testing::utils::equality(res2, v1);

    res2 -= v2;
    res3 =  v1 - v2;

    pbrlib::testing::utils::equality(res2, res3);
}

TEST(MathVec2, Multiplication)
{
    Vec2<float>             v       (2.0f, 3.0f);
    constexpr Vec2<float>   res     (4.0f, 6.0f);
    constexpr float         s       (2.0f);

    pbrlib::testing::utils::equality(res, v * s);

    v *= s;

    pbrlib::testing::utils::equality(res, v);
}

TEST(MathVec2, AccessToElement)
{
    constexpr Vec2<int> v (4, 1232);

    pbrlib::testing::utils::equality(4, v.x);
    pbrlib::testing::utils::equality(1232, v.y);

    pbrlib::testing::utils::equality(4, v[0]);
    pbrlib::testing::utils::equality(1232, v[1]);
}

TEST(MathVec2, Length)
{
    constexpr Vec2<float> v1 (3434.323f, 121.5454f);
    constexpr Vec2<float> v2 (0.132323f, 76.43400f);

    pbrlib::testing::utils::equality(3436.473156098f, v1.length());
    pbrlib::testing::utils::equality(76.43411453f, v2.length());
}

TEST(MathVec2, Normalize)
{
    constexpr Vec2<float> v1 (7643.8738f, 768.434f);
    constexpr Vec2<float> v2 (0.1323230f, 76.4340f);

    constexpr Vec2<float> r1 (0.9949848650f, 0.100025222f);
    constexpr Vec2<float> r2 (0.0017312034f, 0.999998509f);
    
    pbrlib::testing::utils::equality(r1, normalize(v1));
    pbrlib::testing::utils::equality(r2, normalize(v2));
}
