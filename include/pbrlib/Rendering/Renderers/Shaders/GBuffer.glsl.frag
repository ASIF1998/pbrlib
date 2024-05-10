#version 450

#extension GL_GOOGLE_include_directive : enable

#include "MaterialData.h"

layout(location = 0) in highp       vec3 FPosition;
layout(location = 1) in mediump     vec2 FTextureCoord;
layout(location = 2) in highp       mat3 FTBN;

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
    highp vec3 normal   = FTBN * (texture(NormalMap, FTextureCoord).xyz * 2.0f - 1.0f);

    RPositionAndMetallic        = vec4(FPosition, texture(Metallic, FTextureCoord).r);
    RNormalAndRoughness         = vec4(normal, texture(Roughness, FTextureCoord).r);
    RAlbedoAndBakedAO           = vec4(texture(Albedo, FTextureCoord).rgb, texture(AO, FTextureCoord).r);
    RTangentAndAnisotropy       = vec4(vec3(FTBN[0][0], FTBN[1][0], FTBN[2][0]), material_data.data.anisotropy);
}
