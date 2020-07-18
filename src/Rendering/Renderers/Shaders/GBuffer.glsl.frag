#version 450

layout(location = 0) in vec3 FPosition;
layout(location = 1) in vec2 FTextureCoord;
layout(location = 2) in mat3 FTBN;

layout(set = 0, binding = 1) uniform sampler2D Albedo;
layout(set = 0, binding = 2) uniform sampler2D NormalMap;
layout(set = 0, binding = 3) uniform sampler2D Metallic;
layout(set = 0, binding = 4) uniform sampler2D Roughness;
layout(set = 0, binding = 5) uniform sampler2D AO;

layout(location = 0) out vec4 RPositionAndMetallic;
layout(location = 1) out vec4 RNormalAndRoughness;
layout(location = 2) out vec4 RAlbedoAndBakedAO;

void main()
{
    vec3 normal = FTBN * (texture(NormalMap, FTextureCoord).xyz * 2.0f - 1.0f);

    RPositionAndMetallic    = vec4(FPosition, texture(Metallic, FTextureCoord).r);
    RNormalAndRoughness     = vec4(normal, texture(Roughness, FTextureCoord).r);
    RAlbedoAndBakedAO       = vec4(texture(Albedo, FTextureCoord).rgb, texture(AO, FTextureCoord).r);
}