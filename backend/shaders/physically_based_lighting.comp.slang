/// @todo add result image
/// @todo add first openpbr impl

#include <gpu_cpu_constants.h>

#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID 0
#include <gbuffer_generator/exports.h>

#define PBRLIB_MATERIAL_MANAGER_SET_ID 1
#include <material_manager/exports.h>

[[vk::binding(0, 2)]] RWTexture2D<float4> result;

[shader("compute")]
[numthreads(PBRLIB_WORK_GROUP_SIZE, PBRLIB_WORK_GROUP_SIZE, 1)]
void main(uint3 thread_id: SV_DispatchThreadID)
{
    result[thread_id.xy] = float4(1, 1, 0, 1);
}
