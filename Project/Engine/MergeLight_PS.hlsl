#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
};

#define Ambient_Tex     Tex_0
#define Light_Tex       Tex_1

float4 main(VSOut _in) : SV_TARGET
{
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    
    float2 screen_uv = _in.pos.xy / Resolution.xy;
        
    float4 ambient = Ambient_Tex.Sample(Sampler_Point, screen_uv);
    float3 light = Light_Tex.Sample(Sampler_Point, screen_uv).rgb;
    
    float3 final_color = ambient.rgb * 0.5f + light.rgb;
   
    //final_color = final_color / (final_color + float3(1.0f, 1.0f, 1.0f));
    //final_color = pow(final_color, float(1.0f / 2.2f).xxx); // 감마 색상 공간 변환
    
    output.rgb = final_color;
    output.a = 1.f;
    
    return output;
}