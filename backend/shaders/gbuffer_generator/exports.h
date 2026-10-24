#ifndef PBRLIB_GBUFFER_GENERATOR_EXPORTS_GLSL
#define PBRLIB_GBUFFER_GENERATOR_EXPORTS_GLSL

#if defined (PBRLIB_SHADER_GLSL)
    layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 0) uniform sampler2D    gbuffer_pos_uv;
    layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 1) uniform sampler2D    gbuffer_normal_tangent;
    layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 2) uniform usampler2D   gbuffer_material_index;
    layout(set = PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID, binding = 3) uniform sampler2D    gbuffer_depth;
#elif defined (PBRLIB_SHADER_SLANG)
    [[vk::binding(0, PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID)]] Texture2D<float4> gbuffer_pos_uv;
    [[vk::binding(1, PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID)]] Texture2D<float4> gbuffer_normal_tangent;
    [[vk::binding(2, PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID)]] Texture2D<uint> gbuffer_material_index;
    [[vk::binding(3, PBRLIB_GBUFFER_GENERATOR_EXPORTS_SET_ID)]] Texture2D<float> gbuffer_depth;
#endif

#endif
