#version 460

#extension GL_GOOGLE_include_directive                      : enable
#extension GL_EXT_scalar_block_layout                       : enable
#extension GL_EXT_buffer_reference2                         : enable
#extension GL_EXT_shader_16bit_storage                      : enable
#extension GL_EXT_shader_explicit_arithmetic_types_float16  : enable

#define PER_PASS_SET_ID 0

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

struct Globals
{
    mat4 projection_view;
    uint instance_id;
    uint material_index;
};

layout(std430, scalar, buffer_reference, buffer_reference_align = 16) readonly buffer VertexBuffer
{
    Vertex vertices[];
};

layout(push_constant) uniform Block
{
    Globals globals;
};

layout(set = PER_PASS_SET_ID, binding = 0) buffer readonly VertexBuffers
{
    VertexBuffer vertex_buffers[];
};

layout(set = PER_PASS_SET_ID, binding = 1) buffer readonly InstanceData
{
    Instance[] instances;
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