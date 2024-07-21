
//
//  DirectionLightTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../../utils.hpp"

#include <pbrlib/Rendering/Lights/DirectionLight.hpp>

using namespace pbrlib;
using namespace pbrlib::math;

TEST(RenderingLightsDirectionLight, BuilderAndSetters)
{
    Vec3<float>             color           (1.00f, 0.0f, 0.0f);
    Vec3<float>             dir_on_light    (0.00f, 0.0f, 1.0f);
    constexpr float         intensity       (50.0f);

    DirectionLight::Builder build_direction_light;

    build_direction_light.setIntensity(intensity);
    build_direction_light.setDirectionToLight(dir_on_light);
    build_direction_light.setColor(color);
    
    DirectionLight light = build_direction_light.build();

    pbrlib::testing::utils::equality(color, light.getColor());
    pbrlib::testing::utils::equality(dir_on_light, light.getDirectionToLight());
    pbrlib::testing::utils::equality(intensity, light.getIntensity());

    light.setIntensity(intensity * 1232.434f);
    light.setDirectionToLight(dir_on_light * 223.321f);
    light.setColor(color * 3232.323442f);

    color           *= 3232.323442f;
    dir_on_light    *= 223.321f;

    pbrlib::testing::utils::equality(color, light.getColor());
    pbrlib::testing::utils::equality(dir_on_light, light.getDirectionToLight());
    pbrlib::testing::utils::equality(intensity * 1232.434f, light.getIntensity());
}
