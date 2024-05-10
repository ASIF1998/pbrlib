//
//  PointLight.h
//  PBRLib
//
//  Created by Асиф Мамедов on 06/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PointLightData_h
#define PointLightData_h

#ifdef __cplusplus
#   include <pbrlib/math/vec4.hpp>

using vec4 = pbrlib::math::Vec4<float>;
#endif

struct PointLightData
{
    vec4 position;
    vec4 color_and_intensity;
};

#endif /* PointLightData_h */
