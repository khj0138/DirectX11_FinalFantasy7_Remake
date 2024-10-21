#include "Value.hlsli"

struct VSIn
{
    float3 pos : POSITION;
};

struct VSOut
{
    float4 pos : SV_Position;
    float uv_y : POSITION;
};

VSOut main(VSIn _in, uint _id : SV_VertexID)
{
    VSOut output = (VSOut) 0.0f;
    
    float4 pos = mul(mul(float4(_in.pos, 1.f), Mat_View), Mat_Proj);

    if(_id % 2 == 0)
    {
        output.uv_y = 0.0f;
    }
    else
    {
        output.uv_y = 1.0f;
    }
    output.pos = pos;
    
    return output;
}