#version 460

#extension GL_GOOGLE_include_directive  : enable
#extension GL_EXT_scalar_block_layout   : enable
#extension GL_EXT_buffer_reference2     : enable

#include <gbuffer_generator/mesh.glsl>

#define PER_PASS_SET_ID 0

struct Globals
{
    mat4 projection_view;
    uint mesh_index;
    uint material_index;
};

layout(push_constant) uniform Block
{
    Globals globals;
};

layout(set = PER_PASS_SET_ID, binding = 0) buffer readonly VisibleVertexBuffers
{
    VertexBuffer visible_vertex_buffers[];
};

layout(set = PER_PASS_SET_ID, binding = 1) buffer readonly MeshDrawData
{
    MeshDraw[] mesh_draw;
};

layout(location = 0) out flat uint  material_index;
layout(location = 1) out vec3       pos;
layout(location = 2) out vec3       normal;
layout(location = 3) out vec3       tangent;
layout(location = 4) out vec2       uv;

void main()
{
    Vertex      vertex      = visible_vertex_buffers[globals.mesh_index].vertices[gl_VertexIndex];
    MeshDraw    draw_info   = mesh_draw[globals.mesh_index];

    material_index = globals.material_index;

    pos     = vec3(draw_info.model * vec4(vertex.pos.xyz, 1.0));
    normal  = vec3(draw_info.normal * vec4(vec3(vertex.nx, vertex.ny, vertex.nz), 0.0));
    tangent = vec3(draw_info.normal * vec4(vec3(vertex.tx, vertex.ty, vertex.tz), 0.0));
    uv      = vec2(vertex.uvx, vertex.uvy);

    gl_Position = globals.projection_view * vec4(pos, 1.0);
}