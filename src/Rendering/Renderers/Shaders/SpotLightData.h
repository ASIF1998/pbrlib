//
//  SpotLightData.h
//  PBRLib
//
//  Created by Асиф Мамедов on 06/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef SpotLightData_h
#define SpotLightData_h

struct SpotLightData
{
    vec4 position_and_inner_radius;
    vec4 direction_and_outer_radius;
    vec4 color_and_intensity;
};

#endif /* SpotLightData_h */
