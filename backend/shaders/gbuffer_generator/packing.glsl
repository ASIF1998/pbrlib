#ifndef PBRLIB_PACKING_GLSL
#define PBRLIB_PACKING_GLSL

struct GBufferData
{
    vec3 pos;
    vec3 normal;
    vec3 tangent;
    vec2 uv;
    uint material_index;
};

/// https://www.shadertoy.com/view/cljGD1
vec2 octWrap(vec2 v)
{
    vec2 w = 1.0 - abs(v.yx);

    if (v.x < 0.0)
        w.x = -w.x;

    if (v.y < 0.0)
        w.y = -w.y;

    return w;
}

vec2 packUnitVec(vec3 v)
{
    v /= dot(abs(v), vec3(1.0, 1.0, 1.0));

    v.xy = v.z > 0.0 ? v.xy : octWrap(v.xy);
    v.xy = v.xy * 0.5 + 0.5; 

    return v.xy;
}

vec3 unpackUnitVec(vec2 v)
{
    v = v * 2.0 - 1.0;

    vec3    n = vec3(v.x, v.y, 1.0 - abs(v.x) - abs(v.y));
    float   t = max(-n.z, 0.0);

    n.xy += vec2(n.x >= 0.0 ? -t : t, n.y >= 0.0 ? -t : t);

    return normalize(n);
}

uint pack(vec2 value)
{
    const float scale = 65535.0f;

    uvec2 v = uvec2(round(clamp(value, vec2(0.0), vec2(1.0)) * scale));

    return ((v.x & 0xffff) << 0u) | ((v.y & 0xffff) << 16u);
}

vec2 unpack(uint value)
{
    const float inv_scale = 1.0 /  65535.0f;

    uvec2 v = uvec2((value << 16) >> 16, (value << 0) >> 16);

    return vec2(v) * inv_scale;
}

void pack(
    in GBufferData  data,
    out vec4        gbuffer_pos_uv,
    out vec4        gbuffer_normal_tangent,
    out uint        gbuffer_material_index
)
{
    vec2    pack_normal     = packUnitVec(data.normal);
    vec2    pack_tangent    = packUnitVec(data.tangent);
    float   pack_uv         = uintBitsToFloat(pack(data.uv));

    gbuffer_pos_uv          = vec4(data.pos, pack_uv);
    gbuffer_normal_tangent  = vec4(pack_normal, pack_tangent);
    gbuffer_material_index  = data.material_index;
}

GBufferData unpack(
    vec4 gbuffer_pos_uv,
    vec4 gbuffer_normal_tangent,
    uint gbuffer_material_index
)
{
    return GBufferData (
        gbuffer_pos_uv.xyz,
        unpackUnitVec(gbuffer_normal_tangent.xy),
        unpackUnitVec(gbuffer_normal_tangent.zw),
        unpack(floatBitsToUint(gbuffer_pos_uv.w)),
        gbuffer_material_index
    );
}

vec3 unpackPos(in vec4 gbuffer_pos_uv)
{
    return gbuffer_pos_uv.xyz;
}

vec2 unpackUv(in vec4 gbuffer_pos_uv)
{
    return unpack(floatBitsToUint(gbuffer_pos_uv.w));
}

vec3 unpackNormal(in vec4 gbuffer_normal_tangent)
{
    return unpackUnitVec(gbuffer_normal_tangent.xy);
}

vec3 unpackTangent(in vec4 gbuffer_normal_tangent)
{
    return unpackUnitVec(gbuffer_normal_tangent.zw);
}

uint unpackMaterialIndex(in uint gbuffer_material_index)
{
    return gbuffer_material_index;
}

void unpackPosUv(in vec4 gbuffer_pos_uv, out vec3 pos, out vec2 uv)
{
    pos = unpackPos(gbuffer_pos_uv);
    uv  = unpackUv(gbuffer_pos_uv);
}

void unpackNormalTangent(in vec4 gbuffer_normal_tangent, out vec3 normal, out vec3 tangent)
{
    normal  = unpackNormal(gbuffer_normal_tangent);
    tangent = unpackTangent(gbuffer_normal_tangent);
}

#endif