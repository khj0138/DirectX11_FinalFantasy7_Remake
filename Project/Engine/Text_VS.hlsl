#include "Value.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.0f;
  
    float4 positionOS = float4(_in.pos, 1.0f);
               
    output.pos = mul(float4(positionOS.xyz, 1), Mat_World);
    output.color = _in.color;
  
    return output;
}