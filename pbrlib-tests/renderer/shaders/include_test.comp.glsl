#version 450

#extension GL_GOOGLE_include_directive : enable

#include <image_info.glsl>

layout(local_size_x = 16, local_size_y = 61, local_size_z = 1) in;

layout(binding = 0) uniform UniformBuffer {
    ImageInfo image_info;
} ubo;

layout(std430, binding = 1) writeonly buffer OutputBuffer {
    uint data[];
} dst;

void main()
{
    dst.data[0] = ubo.image_info.width;
    dst.data[1] = ubo.image_info.height;
    dst.data[2] = ubo.image_info.format;
    dst.data[3] = ubo.image_info.bpp;
    dst.data[4] = ubo.image_info.mip_levels;
    dst.data[5] = ubo.image_info.sample_count;
}