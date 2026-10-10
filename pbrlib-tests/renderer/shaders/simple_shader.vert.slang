struct SceneParapmeters
{
    float4x4 mvp;
};

ConstantBuffer<SceneParapmeters> scene_params;

struct VSOutput
{
    float4 position_cs   : SV_Position;
    float2 texture_coord : TEXCOORD;
};

[shader("vertex")]
VSOutput main(float3 position_os: POSITION, float2 tex_coord: TEXCOORD)
{
    var pos_cs = mul(scene_params.mvp, float4(position_os, 1));
    VSOutput output = (VSOutput)0;
    output.position_cs      = pos_cs;
    output.texture_coord    = tex_coord;

    return output;
}