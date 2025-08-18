float4 main( float4 pos : POSITION ) : SV_POSITION
{
    return float4(pos.x,pos.y,pos.z,1.0f);
}