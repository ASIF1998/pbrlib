#ifndef PBRLIB_GBUFFER_GENERATOR_EXPORTS_GLSL
#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_GLSL

layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 0) uniform sampler2D    gbuffer_pos_uv;
layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 1) uniform sampler2D    gbuffer_normal_tangent;
layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 2) uniform usampler2D   gbuffer_material_index;
layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 3) uniform sampler2D    gbuffer_depth;

#endif
