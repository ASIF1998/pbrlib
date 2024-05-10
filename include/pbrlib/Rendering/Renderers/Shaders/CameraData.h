//
//  CameraData.h
//  PBRLib
//
//  Created by Асиф Мамедов on 06/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef CameraData_h
#define CameraData_h

#ifdef __cplusplus
#   include <pbrlib/math/vec3.hpp>

using vec3 = pbrlib::math::Vec3<float>;
#endif

struct CameraData
{
    vec3 position;
};

#endif /* CameraData_h */
