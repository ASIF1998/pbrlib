#pragma once

#ifdef PBRLIB_ENABLE_PROPFILING
#   include <vulkan/vulkan.h>
#   include <tracy/Tracy.hpp>
#   include <tracy/TracyVulkan.hpp>
#endif

#ifdef PBRLIB_ENABLE_PROPFILING
#   define PBRLIB_PROFILING_ZONE_SCOPED ZoneScoped
#   define PBRLIB_PROFILING_VK_ZONE_SCOPED(device, cmd_buffer_handle, name) TracyVkZone((device).tracyContext(), cmd_buffer_handle, name)
#else 
#   define PBRLIB_PROFILING_ZONE_SCOPED
#   define PBRLIB_PROFILING_VK_ZONE_SCOPED(device, cmd_buffer_handle, name)
#endif