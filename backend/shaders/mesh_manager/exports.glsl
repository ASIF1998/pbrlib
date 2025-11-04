#ifndef PBRLIB_MESH_MANAGER_EXPORTS_GLSL
#define PBRLIB_MESH_MANAGER_EXPORTS_GLSL

#extension GL_EXT_scalar_block_layout                       : enable
#extension GL_EXT_buffer_reference2                         : enable
#extension GL_EXT_shader_16bit_storage                      : enable
#extension GL_EXT_shader_explicit_arithmetic_types_float16  : enable

struct Vertex
{
    vec4        pos;
    float16_t   nx, ny, nz;
    float16_t   tx, ty, tz;
    float16_t   uvx, uvy;
};

struct Instance
{
    mat4 model;
    mat4 normal;
    uint mesh_id;
};

layout(std430, scalar, buffer_reference, buffer_reference_align = 16) readonly buffer VertexBuffer
{
    Vertex vertices[];
};

layout(set = PBRLIB_MESH_MANAGER_EXPORTS_SET_ID, binding = 0) buffer readonly VertexBuffers
{
    VertexBuffer vertex_buffers[];
};

layout(set = PBRLIB_MESH_MANAGER_EXPORTS_SET_ID, binding = 1) buffer readonly InstanceData
{
    Instance instances[];
};

#endif