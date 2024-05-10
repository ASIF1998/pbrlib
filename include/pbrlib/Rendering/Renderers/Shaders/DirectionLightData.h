//
//  DirectionLightData.h
//  PBRLib
//
//  Created by Асиф Мамедов on 06/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DirectionLightData_h
#define DirectionLightData_h

#ifdef __cplusplus
#   include <pbrlib/math/vec4.hpp>

using vec4 = pbrlib::math::Vec4<float>;
#endif

struct DirectionLightData
{
    vec4 direction_to_light;
    vec4 color_and_intensity;
};

#endif /* DirectionLightData_h */
