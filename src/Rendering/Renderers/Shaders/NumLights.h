//
//  NumLights.h
//  PBRLib
//
//  Created by Асиф Мамедов on 06/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef NumLights_h
#define NumLights_h


#ifdef __cplusplus 
    using uint = uint32_t;
#endif

const uint MAX_POINT_LIGHT      = 1024;
const uint MAX_SPOT_LIGHT       = 1024;
const uint MAX_DIRECTION_LIGHT  = 1024;

struct NumLights
{
    uint point;
    uint spot;
    uint direct;
};

#endif /* NumLights_h */
