
struct VSout
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;
    
};

cbuffer cBuf
{
    row_major matrix transform;
};

VSout main(float2 pos : POSITION, float3 color: COLOR)
{
    VSout vso;
    vso.pos = mul(float4(pos.x, pos.y, 1.0f, 1.0f), transform);
    vso.color = color;
    
    return vso;
}