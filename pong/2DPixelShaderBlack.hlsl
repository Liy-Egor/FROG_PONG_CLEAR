float4 main(float3 color : COLOR) : SV_TARGET
{
    return float4(color.x, color.y, color.z, 1.0f);
}