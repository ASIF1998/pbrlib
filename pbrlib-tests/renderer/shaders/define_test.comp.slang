StructuredBuffer<float> buffer_0;

#ifdef PBRLIB_DEFINE_TEST
StructuredBuffer<float> buffer_1;
#endif

RWStructuredBuffer<float> result;

[shader("compute")]
[numthreads(16, 16, 1)]
void main(uint3 thread_id: SV_DispatchThreadID)
{
    uint index = thread_id.x;

#ifdef PBRLIB_DEFINE_TEST
    result[index] = buffer_0[index] + buffer_1[index];
#else
    result[index] = buffer_0[index];
#endif
}
