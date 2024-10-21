#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float uv_y : POSITION;
};

#define Color Vec4_0

float4 main(VSOut _in) : SV_Target
{
    float2 uv = _in.pos.xy / Resolution.xy;
		
    float4 color = Tex_0.Sample(Sampler_Anisotropic, uv);
    color.rgb += (Color.rgb * _in.uv_y);
    //color.rgb /= 2.0f;
    return color;
}