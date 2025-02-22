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

#endif