#ifndef MESH_GLSL
#define MESH_GLSL

struct Vertex
{
    vec4 pos;
    vec4 normal;
    vec4 tangent;
    vec4 uv;
};

struct MeshDraw
{
    mat4 model_view;
    mat3 normal;
    uint material_index;
};

layout(std430, scalar, buffer_reference, buffer_reference_align = 16) readonly buffer VertexBuffer
{
    Vertex[] vertices;
};

layout(std430, scalar, buffer_reference, buffer_reference_align = 16) readonly buffer IndexBuffer
{
    uint[] indices;
};

#endif