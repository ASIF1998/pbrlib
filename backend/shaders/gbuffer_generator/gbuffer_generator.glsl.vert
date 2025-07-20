#version 460

#extension GL_GOOGLE_include_directive : enable

#define PBRLIB_MESH_MANAGER_EXPORTS_SET_ID 0
#include <mesh_manager/exports.glsl>

struct Globals
{
    mat4 projection_view;
    uint instance_id;
    uint material_index;
};

layout(push_constant) uniform Block
{
    Globals globals;
};

layout(location = 0) out flat uint  material_index;
layout(location = 1) out vec3       pos;
layout(location = 2) out vec3       normal;
layout(location = 3) out vec3       tangent;
layout(location = 4) out vec2       uv;

void main()
{
    Instance    instance    = instances[globals.instance_id];
    Vertex      vertex      = vertex_buffers[instance.mesh_id].vertices[gl_VertexIndex];

    material_index = globals.material_index;

    pos     = vec3(instance.model * vec4(vertex.pos.xyz, 1.0));
    normal  = vec3(instance.normal * vec4(vec3(vertex.nx, vertex.ny, vertex.nz), 0.0));
    tangent = vec3(instance.normal * vec4(vec3(vertex.tx, vertex.ty, vertex.tz), 0.0));
    uv      = vec2(vertex.uvx, vertex.uvy);

    gl_Position = globals.projection_view * vec4(pos, 1.0);
}