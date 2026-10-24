/// @todo add material manager descriptor set
/// @todo add first openpbr impl

#include <gpu_cpu_constants.h>

#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID 0
#include <gbuffer_generator/exports.h>

[shader("compute")]
[numthreads(PBRLIB_WORK_GROUP_SIZE, PBRLIB_WORK_GROUP_SIZE, 1)]
void main(uint3 thread_id: SV_DispatchThreadID)
{

}
