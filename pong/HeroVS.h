cbuffer global : register(b5)
{
    float4 gConst[32];
};

cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
};

cbuffer camera : register(b3)
{
    float4x4 world[2];
    float4x4 view[2];
    float4x4 proj[2];
};

cbuffer drawMat : register(b2)
{
    float4x4 model;
    float hilight;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float4 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

float3 rotY(float3 pos, float a)
{
    float3x3 m =
    {
        cos(a), 0, sin(a),
        0, 1, 0,
        -sin(a), 0, cos(a)
    };
    pos = mul(pos, m);
    return pos;
}


VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    // Извлекаем параметры из константного буфера
    float left = gConst[0].x;
    float top = gConst[0].y;
    float right = gConst[0].z;
    float bottom = gConst[0].w;

    // Верный порядок вершин (как в рабочем шейдере)
    float2 positions[6] = {
        float2(left, bottom),   // Нижний левый (v0)
        float2(right, bottom),  // Нижний правый (v1)
        float2(left, top),      // Верхний левый (v2)

        float2(right, bottom),  // Нижний правый (v3 = v1)
        float2(right, top),     // Верхний правый (v4)
        float2(left, top)       // Верхний левый (v5 = v2)
    };

    // Автоматический расчёт UV (как в рабочем шейдере)
    float2 p = positions[vID];
    output.pos = float4(p, 0.0f, 1.0f);
    float2 uvCoords[6] = {
    float2(0, 1), // Верхний левый
    float2(1, 1), // Верхний правый
    float2(0, 0), // Нижний левый

    float2(1, 1), // Верхний правый
    float2(1, 0), // Нижний правый
    float2(0, 0)  // Нижний левый
    };

    output.uv = uvCoords[vID];
    return output;
}