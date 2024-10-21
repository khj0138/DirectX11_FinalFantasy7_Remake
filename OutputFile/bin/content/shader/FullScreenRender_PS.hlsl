#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(VSOut _in) : SV_TARGET
{
    float4 output = (float4) 0.0f;
    
    output = Tex_0.Sample(Sampler_Linear, _in.uv);
    
    return output;
}