#ifndef MESH_GLSL
#define MESH_GLSL

#extension GL_EXT_shader_16bit_storage:                     enable
#extension GL_EXT_shader_explicit_arithmetic_types_float16: enable

struct Vertex
{
    vec4        pos;
    float16_t   nx, ny, nz;
    float16_t   tx, ty, tz;
    float16_t   uvx, uvy;
};

layout(std430, scalar, buffer_reference, buffer_reference_align = 16) readonly buffer VertexBuffer
{
    Vertex vertices[];
};

struct MeshDraw
{
    mat4 model;
    mat4 normal;
};

#endif