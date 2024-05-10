//
//  core.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef core_h
#define core_h

#include <vulkan/vulkan.h>

#include <cmath>

#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

namespace pbrlib
{
    constexpr uint32_t EngineVersion = VK_MAKE_VERSION(
        0,
        0,
        1
    );

    constexpr uint32_t VulkanVersion = VK_MAKE_VERSION(
        1,
        3,
        0
    );
}

#endif /* core_h */
