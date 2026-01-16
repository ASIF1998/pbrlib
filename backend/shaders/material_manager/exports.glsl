#ifndef PBRLIB_MATERIAL_MANAGER_EXPORTS_GLSL
#define PBRLIB_MATERIAL_MANAGER_EXPORTS_GLSL

struct Material
{
    uint albedo;
    uint normal_map;
    uint metallic;
    uint roughness;
};

layout(set = PBRLIB_MATERIAL_MANAGER_SET_ID, binding = 0) uniform sampler2D materials_images[];

layout(set = PBRLIB_MATERIAL_MANAGER_SET_ID, binding = 1) buffer readonly MaterialBuffer
{
    Material materials[];
};

#endif
