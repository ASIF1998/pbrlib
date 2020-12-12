//
//  SpotLightTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../../src/Rendering/Lights/SpotLight.hpp"

using namespace testing;
using namespace pbrlib;
using namespace pbrlib::math;

TEST(RenderingLightsSpotLight, BuilderAndSetters)
{
    constexpr Vec3<float>   color           (1.00f, 0.0f, 0.00f);
    constexpr Vec3<float>   pos             (0.50f, 0.5f, 0.50f);
    constexpr Vec3<float>   dir             (0.00f, 0.0f, -1.0f);
    constexpr float         intensity       (50.0f);
    constexpr float         outer_radius    (20.0f);
    constexpr float         inner_radius    (10.0f);

    SpotLight::Builder build_spot_light;

    build_spot_light.setPosition(pos);
    build_spot_light.setOuterRadius(outer_radius);
    build_spot_light.setInnerRadius(inner_radius);
    build_spot_light.setIntensity(intensity);
    build_spot_light.setDirection(dir);
    build_spot_light.setColor(color);

    SpotLight light = build_spot_light.build();

    EXPECT_EQ(color, light.getColor());
    EXPECT_EQ(pos, light.getPosition());
    EXPECT_EQ(dir, light.getDirection());
    EXPECT_EQ(intensity, light.getIntensity());
    EXPECT_EQ(outer_radius, light.getOuterRadius());
    EXPECT_EQ(inner_radius, light.getInnerRadius());

    light.setPosition(pos * 21.323f);
    light.setOuterRadius(outer_radius * 434.54f);
    light.setInnerRadius(inner_radius * 12.343f);
    light.setIntensity(intensity * 123.3223f);
    light.setDirection(dir * 0.003232f);
    light.setColor(color * 0.00323653f);

    EXPECT_EQ(color * 0.00323653f, light.getColor());
    EXPECT_EQ(pos * 21.323f, light.getPosition());
    EXPECT_EQ(dir * 0.003232f, light.getDirection());
    EXPECT_EQ(intensity * 123.3223f, light.getIntensity());
    EXPECT_EQ(outer_radius * 434.54f, light.getOuterRadius());
    EXPECT_EQ(inner_radius * 12.343f, light.getInnerRadius());
}
