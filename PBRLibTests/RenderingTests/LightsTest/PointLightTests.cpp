//
//  PointLightTests.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../../src/Rendering/Lights/PointLight.hpp"

using namespace testing;
using namespace pbrlib;
using namespace pbrlib::math;

TEST(RenderingLightsPointLight, BuilderAndSetters)
{
    constexpr Vec3<float>   color       (1.00f, 0.0f, 0.0f);
    constexpr Vec3<float>   pos         (0.05f, 0.5f, 0.5f);
    constexpr float         intensity   (50.0f);

    PointLight::Builder build_point_light;

    build_point_light.setColor(color);
    build_point_light.setPosition(pos);
    build_point_light.setIntensity(intensity);

    PointLight light = build_point_light.build();

    EXPECT_EQ(color, light.getColor());
    EXPECT_EQ(pos, light.getPosition());
    EXPECT_EQ(intensity, light.getIntensity());

    light.setColor(color * 1.5433f);
    light.setIntensity(intensity * 0.434f);
    light.setPosition(pos * 21.43434f);

    EXPECT_EQ(color * 1.5433f, light.getColor());
    EXPECT_EQ(pos * 21.43434f, light.getPosition());
    EXPECT_EQ(intensity * 0.434f, light.getIntensity());
}