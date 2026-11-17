/// @todo add first openpbr impl

#include <gpu_cpu_constants.h>

[[vk::binding(0, 0)]] RWTexture2D<float4> result;

#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID 1
#include <gbuffer_generator/exports.h>
#include <gbuffer_generator/packing.h>

#define PBRLIB_MATERIAL_MANAGER_SET_ID 2
#include <material_manager/exports.h>

#define PBRLIB_LIGHT_MANAGER_EXPORTS_SET_ID 3
#include <lights_manager/exports.h>

struct PushConstants
{
    float3 camera_direction;
};

[shader("compute")]
[numthreads(PBRLIB_WORK_GROUP_SIZE, PBRLIB_WORK_GROUP_SIZE, 1)]
void main(uint3 thread_id: SV_DispatchThreadID, uniform PushConstants push_constants)
{
    uint width  = 0;
    uint height = 0;

    gbuffer_pos_uv.GetDimensions(width, height);

    let screen_uv = float2(thread_id.xy) / float2(width, height);

    let gbuffer_data = unpack (
        gbuffer_pos_uv.Sample(screen_uv),
        gbuffer_normal_tangent.Sample(screen_uv),
        gbuffer_material_index.Sample(screen_uv)
    );

    let material_index  = gbuffer_material_index.Sample(screen_uv);
    let material        = materials[material_index];
    let color           = materials_images[material.albedo].Sample(gbuffer_data.uv);

    result[thread_id.xy] = color * dot(push_constants.camera_direction, gbuffer_data.normal);

    uint stride = 0;
    uint count = 0;
    point_lights.GetDimensions(count, stride);
    if (count > 0)
    {
        let point_light = point_lights[0];
        result[thread_id.xy].xyz = lerp(result[thread_id.xy].xyz, point_light.color, 0.6);
    }
}
