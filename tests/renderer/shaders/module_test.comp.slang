import image_info;

ConstantBuffer<ImageInfo>       image_info;
RWStructuredBuffer<uint32_t>    out_buffer;

[shader("compute")]
[numthreads(16, 16, 1)]
void main(uint3 thread_id: SV_DispatchThreadID)
{
    out_buffer[0] = image_info.width;
    out_buffer[1] = image_info.height;
    out_buffer[2] = image_info.format;
    out_buffer[3] = image_info.bpp;
    out_buffer[4] = image_info.mip_levels;
    out_buffer[5] = image_info.sample_count;
}
