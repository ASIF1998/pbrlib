#ifndef PBRLIB_LIGHT_MANAGER_EXPORT
#define PBRLIB_LIGHT_MANAGER_EXPORT

#include <lights_manager/lights.h>

[[vk::binding(0, PBRLIB_LIGHT_MANAGER_EXPORTS_SET_ID)]] StructuredBuffer<pbrlib::gpu_cpu_interrop::PointLight> point_lights;

#endif
