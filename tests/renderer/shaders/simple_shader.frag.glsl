#version 450

layout(location = 0) in     vec2 in_texture_coord;
layout(location = 0) out    vec4 final_color;

layout(set = 1, binding = 1) uniform sampler2D color_image;

void main()
{
    vec3 color = pow(texture(color_image, in_texture_coord).rgb, vec3(0.4545));

    final_color = vec4(color, 1.0);
}