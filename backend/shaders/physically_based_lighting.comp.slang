/// @todo add first openpbr impl

#include <gpu_cpu_constants.h>

[[vk::binding(0, 0)]] RWTexture2D<float4> result;

#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID 1
#include <gbuffer_generator/exports.h>

#define PBRLIB_MATERIAL_MANAGER_SET_ID 2
#include <material_manager/exports.h>

float2 unpackMeshUv(uint packed)
{
    let inv_scale = 1.0 / 65535.0;
    let v         = uint2((packed << 16) >> 16, packed >> 16);

    return float2(v) * inv_scale;
}

[shader("compute")]
[numthreads(PBRLIB_WORK_GROUP_SIZE, PBRLIB_WORK_GROUP_SIZE, 1)]
void main(uint3 thread_id: SV_DispatchThreadID)
{
    uint width  = 0;
    uint height = 0;

    gbuffer_pos_uv.GetDimensions(width, height);

    let screen_uv       = float2(thread_id.xy) / float2(width, height);
    let mesh_uv         = unpackMeshUv(asuint(gbuffer_pos_uv.Sample(screen_uv).w));
    let material_index  = gbuffer_material_index.Sample(screen_uv);
    let material        = materials[material_index];
    let color           = materials_images[material.albedo].Sample(mesh_uv);

    result[thread_id.xy] = color;
}
