#version 460

layout(location = 0) in flat uint material_index;
layout(location = 1) in vec3 pos;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec2 uv;

layout(location = 0) out vec4 gbuffer_0;

void main()
{
    gbuffer_0 = vec4(1);
}