#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texture_coord;

layout(location = 0) out vec2 out_texture_coord;

layout(set = 0, binding = 0) uniform UB
{
    mat4 MVP;
} ub;

void main()
{
    gl_Position = ub.MVP * vec4(in_position, 1);
    out_texture_coord = in_texture_coord;
}
