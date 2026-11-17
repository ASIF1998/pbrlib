#ifndef PBRLIB_IMAGE_INFO_GLSL
#define PBRLIB_IMAGE_INFO_GLSL

struct ImageInfo
{
    uint width;
    uint height;
    uint format;
    uint bpp;
    uint mip_levels;
    uint sample_count;
};

#endif