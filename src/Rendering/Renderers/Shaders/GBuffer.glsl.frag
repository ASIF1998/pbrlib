#version 450

#extension GL_GOOGLE_include_directive : enable

#include "MaterialData.h"

// layout(location = 0) in highp       vec3 FPosition;
// layout(location = 1) in mediump     vec2 FTextureCoord;

// layout(location = 2) in highp       mat3 FTBN;

layout(location = 0) in highp      vec3 FPosition;
layout(location = 1) in mediump    vec2 FTextureCoord;

layout(location = 2) in highp      vec3 FNorm;
layout(location = 3) in highp      vec3 FTang;
layout(location = 4) in highp      vec3 FBtang;

layout(set = 0, binding = 1) uniform sampler2D Albedo;
layout(set = 0, binding = 2) uniform sampler2D NormalMap;
layout(set = 0, binding = 3) uniform sampler2D Metallic;
layout(set = 0, binding = 4) uniform sampler2D Roughness;
layout(set = 0, binding = 5) uniform sampler2D AO;

layout(location = 0) out highp  vec4 RPositionAndMetallic;
layout(location = 1) out highp  vec4 RNormalAndRoughness;
layout(location = 2) out lowp   vec4 RAlbedoAndBakedAO;
layout(location = 3) out highp  vec4 RTangentAndAnisotropy;

layout(std140, binding = 6) uniform u_material_data
{
    MaterialData data;
} material_data;

void main()
{
    // highp mat3 TBN      = mat3(FTang, FBtang, FNorm);
    highp mat3 TBN      = transpose(mat3(FTang, FBtang, FNorm));
    highp vec3 normal   = TBN * (texture(NormalMap, FTextureCoord).xyz * 2.0f - 1.0f);

    RPositionAndMetallic        = vec4(FPosition, texture(Metallic, FTextureCoord).r);
    RNormalAndRoughness         = vec4(normal, texture(Roughness, FTextureCoord).r);
    RAlbedoAndBakedAO           = vec4(texture(Albedo, FTextureCoord).rgb, texture(AO, FTextureCoord).r);
    // RTangentAndAnisotropy       = vec4(vec3(TBN[0][0], TBN[0][1], TBN[0][2]), material_data.data.anisotropy);
    RTangentAndAnisotropy       = vec4(FTang, material_data.data.anisotropy);
}
