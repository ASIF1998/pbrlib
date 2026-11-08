#ifndef PBRLIB_LIGHTS
#define PBRLIB_LIGHTS

/// @todo add support for slang

#ifndef PBRLIB_SHADER_SLANG
#   include <pbrlib/math/vec3.hpp>
#endif

namespace pbrlib::gpu_cpu_interrop
{
    struct PointLight
    {
        math::vec3  color;
        float       intensity;
        math::vec3  position_ws;
    };
}

#endif
