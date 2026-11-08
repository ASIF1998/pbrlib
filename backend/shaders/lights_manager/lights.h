#ifndef PBRLIB_LIGHT_MANAGER_LIGHTS
#define PBRLIB_LIGHT_MANAGER_LIGHTS

#ifndef PBRLIB_SHADER_SLANG
#   include <pbrlib/math/vec3.hpp>
#endif

#ifdef PBRLIB_SHADER_SLANG
#   define PBRLIB_POINT_LIGHT_ALIGNAS
#else
#   define PBRLIB_POINT_LIGHT_ALIGNAS alignas(16)
#endif

#ifdef PBRLIB_SHADER_SLANG
namespace pbrlib::math
{
    typealias vec3 = float3;
}
#endif

namespace pbrlib::gpu_cpu_interrop
{
    struct PBRLIB_POINT_LIGHT_ALIGNAS PointLight
    {
        math::vec3  color;
        float       intensity;
        math::vec3  position_ws;
    };
}

#endif
