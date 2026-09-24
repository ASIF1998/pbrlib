#version 460

#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in flat uint   material_index;
layout(location = 1) in vec3        pos;
layout(location = 2) in vec3        normal;
layout(location = 3) in vec3        tangent;
layout(location = 4) in vec2        uv;

layout(location = 0) out vec4 gbuffer_pos_uv;
layout(location = 1) out vec4 gbuffer_normal_tangent;
layout(location = 2) out uint gbuffer_material_index;

#include <gbuffer_generator/packing.glsl>

void main()
{
    GBufferData data = GBufferData (
        pos,
        normal,
        tangent,
        uv,
        material_index
    );

    pack(data, gbuffer_pos_uv, gbuffer_normal_tangent, gbuffer_material_index);
}
