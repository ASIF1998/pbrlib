enum Selector
{
    First = 0,
    Second
};

[UnscopedEnum]
enum Bump
{
    NoBump      = 0,
    ProcBump    = 1
};

namespace pbrlib::test
{
    float4 add(float4 a, float4 b)
    {
        return a + b;
    }

    float4 Sub(float4 a, float4 b)
    {
        return a - b;
    }
}

[shader("fragment")]
float4 main()
{
    return float4(1, 0, 0, 1);
}