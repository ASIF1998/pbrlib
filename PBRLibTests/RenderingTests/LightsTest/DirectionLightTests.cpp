
//
//  DirectionLightTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../../src/Rendering/Lights/DirectionLight.hpp"

using namespace testing;
using namespace pbrlib;
using namespace pbrlib::math;

TEST(RenderingLightsDirectionLight, BuilderAndSetters)
{
    constexpr Vec3<float>   color           (1.0f, 0.0f, 0.0f);
    constexpr Vec3<float>   dir_on_light    (0.0f, 0.0f, 1.0f);
    constexpr float         intensity       (50.0f);

    DirectionLight::Builder build_direction_light;

    build_direction_light.setIntensity(intensity);
    build_direction_light.setDirectionOnLight(dir_on_light);
    build_direction_light.setColor(color);
    
    DirectionLight light = build_direction_light.build();

    EXPECT_EQ(color, light.getColor());
    EXPECT_EQ(dir_on_light, light.getDirectionOnLight());
    EXPECT_EQ(intensity, light.getIntensity());

    light.setIntensity(intensity * 1232.434f);
    light.setDirectionOnLight(dir_on_light * 223.321f);
    light.setColor(color * 3232.323442f);

    EXPECT_EQ(color * 3232.323442f, light.getColor());
    EXPECT_EQ(dir_on_light * 223.321f, light.getDirectionOnLight());
    EXPECT_EQ(intensity * 1232.434f, light.getIntensity());
}
