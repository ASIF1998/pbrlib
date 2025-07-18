#ifndef GENERATION_GLSL
#define GENERATION_GLSL

vec3 randomVec(vec3 v)
{
    const uint k = 1103515245;

    uvec3 x = floatBitsToUint(v);

    x = ((x >> 8u) ^ x.yzx) * k;
    x = ((x >> 8u) ^ x.yzx) * k;
    x = ((x >> 8u) ^ x.yzx) * k;

    return normalize(vec3(x));
}

#endif