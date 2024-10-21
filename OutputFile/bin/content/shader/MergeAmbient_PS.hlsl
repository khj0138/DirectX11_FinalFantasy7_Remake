#include "Value.hlsli"
#include "BRDF.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
};

#define Color_Metalic       Tex_0
#define Normal_Roughness    Tex_1
#define Position            Tex_2
#define Emissive_occlusion  Tex_3
#define Light Tex_4

float4 main(VSOut _in) : SV_TARGET
{
    float2 screen_uv = _in.pos.xy / Resolution.xy;
    
    float4 out_color = (float4) 0.0f;
    float3 ambient = Color_Metalic.Sample(Sampler_Point, screen_uv).rgb;
    float3 color = Light.Sample(Sampler_Point, screen_uv).rgb;
    float3 emissive = Emissive_occlusion.Sample(Sampler_Point, screen_uv).rgb;
    
    //float occlusion = Emissive_occlusion.Sample(Sampler_Anisotropic, screen_uv).a;
    
    out_color.rgb = ambient /** 0.3f*/ + color + emissive;
    
    return float4(out_color.rgb, 1.0f);
}