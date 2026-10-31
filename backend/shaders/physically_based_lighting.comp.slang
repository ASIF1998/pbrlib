/// @todo add first openpbr impl

#include <gpu_cpu_constants.h>

[[vk::binding(0, 0)]] RWTexture2D<float4> result;

#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID 1
#include <gbuffer_generator/exports.h>

#define PBRLIB_MATERIAL_MANAGER_SET_ID 2
#include <material_manager/exports.h>

[shader("compute")]
[numthreads(PBRLIB_WORK_GROUP_SIZE, PBRLIB_WORK_GROUP_SIZE, 1)]
void main(uint3 thread_id: SV_DispatchThreadID)
{
    uint width  = 0;
    uint height = 0;

    gbuffer_pos_uv.GetDimensions(width, height);

    float2 uv = float2(thread_id.xy) / float2(width, height);

    const uint material_index = gbuffer_material_index.Sample(uv);
    float4 color = float4(0);

    if (material_index > 0)
    {
        const Material material = materials[material_index - 1];
        color = materials_images[material.albedo].Sample(uv);
    }

    result[thread_id.xy] = color;
}
