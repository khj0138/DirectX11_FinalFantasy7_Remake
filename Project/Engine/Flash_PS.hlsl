#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(VSOut _in) : SV_Target
{
    float4 out_color = Tex_0.Sample(Sampler_Point, _in.uv);
    out_color.rgb += Float_0;
    
    return out_color;

}
