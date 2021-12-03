//
//  PointLightTests.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../../utils.hpp"

#include "../../../src/Rendering/Lights/PointLight.hpp"

using namespace pbrlib;
using namespace pbrlib::math;

TEST(RenderingLightsPointLight, BuilderAndSetters)
{
    Vec3<float>             color       (1.00f, 0.0f, 0.0f);
    Vec3<float>             pos         (0.05f, 0.5f, 0.5f);
    constexpr float         intensity   (50.0f);

    PointLight::Builder build_point_light;

    build_point_light.setColor(color);
    build_point_light.setPosition(pos);
    build_point_light.setIntensity(intensity);

    PointLight light = build_point_light.build();

    pbrlib::testing::utils::equality(color, light.getColor());
    pbrlib::testing::utils::equality(pos, light.getPosition());
    pbrlib::testing::utils::equality(intensity, light.getIntensity());

    light.setColor(color * 1.5433f);
    light.setIntensity(intensity * 0.434f);
    light.setPosition(pos * 21.43434f);

    color   *= 1.5433f;
    pos     *= 21.43434f;

    pbrlib::testing::utils::equality(color, light.getColor());
    pbrlib::testing::utils::equality(pos, light.getPosition());
    pbrlib::testing::utils::equality(intensity * 0.434f, light.getIntensity());
}