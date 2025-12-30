#ifndef PBRLIB_FILTER_GLSL
#define PBRLIB_FILTER_GLSL

layout(set = PBRLIB_FILTER_SET_ID, binding = 0) uniform sampler2D          input_image;
layout(set = PBRLIB_FILTER_SET_ID, binding = 1) uniform writeonly image2D  result;

#endif