
struct VSout
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;
    
};

cbuffer cBuf
{
    matrix transform;
};

VSout main(float3 pos : POSITION, float3 color: COLOR)
{
    VSout vso;
    vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
    vso.color = color;
    
    return vso;
}