#version 460

#extension GL_GOOGLE_include_directive                      : enable
#extension GL_EXT_shader_16bit_storage                      : enable
#extension GL_EXT_shader_explicit_arithmetic_types_float16  : enable

#define PBRLIB_FILTER_SET_ID 0
#include <filter.glsl>

#include <math.glsl>

#include <gpu_cpu_constants.h>
layout (local_size_x = PBRLIB_WORK_GROUP_SIZE, local_size_y = PBRLIB_WORK_GROUP_SIZE) in;

shared f16vec4 colors_cache[PBRLIB_WORK_GROUP_SIZE * 2][PBRLIB_WORK_GROUP_SIZE * 2];

struct Settings
{
    uint    sample_count;
    float   sigma_s;
    float   sigma_l;
};

layout(push_constant) uniform Configuration
{
    Settings settings;
};

float16_t luma(f16vec4 color)
{
    return dot(color.rgb, f16vec3(0.299, 0.587, 0.114));
}

ivec2 csign(ivec2 v)
{
    return ivec2(step(0, vec2(v)) * 2.0 - 1.0);
}

void initColorsCache(vec2 uv, vec2 scale)
{
    const int   half_size   = PBRLIB_WORK_GROUP_SIZE >> 1;
    const ivec2 half_size_2 = ivec2(half_size);

    ivec2 local_pos = ivec2(gl_LocalInvocationID.xy);

    ivec2 pos_1 = local_pos + half_size_2;
    ivec2 pos_2 = pos_1 + csign(local_pos - half_size_2) * half_size_2;

    colors_cache[pos_1.x][pos_1.y] = f16vec4(texture(input_image, uv));
    colors_cache[pos_2.x][pos_1.y] = f16vec4(texture(input_image, uv + scale * vec2(pos_2.x - pos_1.x, 1)));
    colors_cache[pos_1.x][pos_2.y] = f16vec4(texture(input_image, uv + scale * vec2(1, pos_2.y - pos_1.y)));

    bool corner_x = any(equal(local_pos.xx, ivec2(0, PBRLIB_WORK_GROUP_SIZE - 1)));
    bool corner_y = any(equal(local_pos.yy, ivec2(0, PBRLIB_WORK_GROUP_SIZE - 1)));

    if (corner_x && corner_y)
    {
        ivec2 pos = local_pos + half_size_2 * ivec2(greaterThan(local_pos, ivec2(0)));
        for (int i = 0; i < half_size; ++i)
        {
            for (int j = 0; j < half_size; ++j)
                colors_cache[pos.x + i][pos.y + j] = f16vec4(texture(input_image, uv + scale * (vec2(i, j) - half_size_2)));
        }
    }

    barrier();
}

void main()
{
    ivec2   pixel_coord = ivec2(gl_GlobalInvocationID.xy);
    vec2    scale       = vec2(1.0) / vec2(textureSize(input_image, 0));
    vec2    uv          = vec2(pixel_coord) * scale;

    const int offset_in_sm = PBRLIB_WORK_GROUP_SIZE >> 1;

    initColorsCache(uv, scale);

    const float16_t fac_s = float16_t(-1.0 / (2.0 * settings.sigma_s * settings.sigma_s));
    const float16_t fac_l = float16_t(-1.0 / (2.0 * settings.sigma_l * settings.sigma_l));

    float16_t l = luma(colors_cache[gl_LocalInvocationID.x + offset_in_sm][gl_LocalInvocationID.y + offset_in_sm]);

    f16vec4     color           = f16vec4(0);
    float16_t   total_weight    = float16_t(0.0);

    int sample_count = int(settings.sample_count >> 1);
    for (int i = -sample_count; i < sample_count; ++i)
    {
        for (int j = -sample_count; j < sample_count; ++j)
        {
            ivec2   offset          = ivec2(i, j) + ivec2(gl_LocalInvocationID.xy) + offset_in_sm;
            f16vec4 offset_color    = colors_cache[offset.x][offset.y];

            float16_t dist_s = length(f16vec2(i, j));
            float16_t dist_l = luma(offset_color) - l;

            float16_t weight = exp((fac_s * dist_s * dist_s) + (fac_l * dist_l * dist_l));

            total_weight    += weight;
            color           += offset_color * weight;
        }
    }

    color /= max(total_weight, float16_t(0.001));
    imageStore(result, pixel_coord, color);
}
