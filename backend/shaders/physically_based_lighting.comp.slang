/// @todo add first openpbr impl

#include <gpu_cpu_constants.h>

#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID 0
#include <gbuffer_generator/exports.h>

#define PBRLIB_MATERIAL_MANAGER_SET_ID 1
#include <material_manager/exports.h>

[shader("compute")]
[numthreads(PBRLIB_WORK_GROUP_SIZE, PBRLIB_WORK_GROUP_SIZE, 1)]
void main(uint3 thread_id: SV_DispatchThreadID)
{

}
