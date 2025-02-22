#version 450

#extension GL_EXT_buffer_reference      : enable
#extension GL_EXT_scalar_block_layout   : enable
#extension GL_GOOGLE_include_directive  : enable

#include <gbuffer_generator/mesh.glsl>

#define PER_PASS_SET_ID 0

struct Globals
{
    mat4 projection;
    uint mesh_index;
};

layout(push_constant) uniform Block
{  
	Globals globals;
};

layout(std430, set = PER_PASS_SET_ID, binding = 0) readonly buffer SceneIndexBuffers
{
    IndexBuffer[] index_buffers;
};

layout(std430, set = PER_PASS_SET_ID, binding = 1) readonly buffer SceneVertexBuffers
{
    VertexBuffer[] vertex_buffers;
};

layout(std430, set = PER_PASS_SET_ID, binding = 2) readonly buffer DrawData
{
    MeshDraw[] mesh_draw_infos;
};

layout(location = 0) out flat uint material_index;
layout(location = 1) out vec3 pos;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 tangent;
layout(location = 4) out vec2 uv;

void main()
{
    uint        index       = index_buffers[globals.mesh_index].indices[gl_VertexIndex];
    Vertex      vertex      = vertex_buffers[globals.mesh_index].vertices[index];
    MeshDraw    mesh_draw   = mesh_draw_infos[globals.mesh_index];

    material_index  = 0;
    pos             = vec3(1);
    normal          = vec3(1);
    tangent         = vec3(1);
    uv              = vec2(1);

    // gl_Position = vec4(vertex.pos.xyz, 1.0);
    gl_Position = vec4(1);
}