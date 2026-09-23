#version 450

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(binding = 0) uniform UniformData {
    uint multiplier;
} ubo;

layout(std430, binding = 1) readonly buffer InputBuffer {
    float data[];
} src;

layout(std430, binding = 2) writeonly buffer OutputBuffer {
    float data[];
} dst;

layout(push_constant) uniform PushConstants {
    uint offset;
    float threshold;
} pcs;

void main()
{
    uint globalIdx = gl_GlobalInvocationID.x + pcs.offset;

    float value = src.data[globalIdx];
    float result = 0.0;

    if (value > pcs.threshold)
    {
        for (uint i = 0; i < ubo.multiplier; ++i)
        {
            result += value * 2.5;
        }
    }
    else
    {
        result = value * 0.5;
    }

    dst.data[globalIdx] = result;
}
