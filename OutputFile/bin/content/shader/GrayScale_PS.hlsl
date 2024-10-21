#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(VSOut _in) : SV_Target
{
    float4 out_color = Tex_0.Sample(Sampler_Point, _in.uv);
    
    float gray = dot(out_color.rgb, float3(0.299, 0.587, 0.114));
    
    float factor = 0.5;
    float3 desaturated = lerp(float3(gray, gray, gray), out_color.rgb, factor);
    
    return float4(desaturated, out_color.a);
}