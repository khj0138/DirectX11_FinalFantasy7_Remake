#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    
    float3 view_pos : POSITION;
    
    float3 view_normal : NORMAL;
    float3 view_tangent : TANGENT;
    float3 view_binormal : BINORMAL;

};
#define Blur_Tex Tex_0
#define Lens_Tex Tex_1
#define UV_Tex Tex_2

float4 main(VSOut _in) : SV_TARGET
{
    float2 uv = _in.pos.xy / Resolution.xy;
    
    float3 out_color = Blur_Tex.Sample(Sampler_Anisotropic, uv).rgb;
    
    if (BTex_1)
    {
        out_color += Lens_Tex.Sample(Sampler_Anisotropic, _in.uv).rgb;
    }
    
    if(BTex_2)
    {
         out_color += (UV_Tex.Sample(Sampler_Anisotropic, _in.uv).rgb * 0.3f);
       // float3 t = (UV_Tex.Sample(Sampler_Anisotropic, _in.uv).rgb);
       // t.xy -= 0.5f;
       // uv += t.xy;
       // out_color += Blur_Tex.Sample(Sampler_Anisotropic, uv).rgb;
    }
    //out_color.rgb = float3(1.0f, 0.0f, 1.0f);
    return float4(out_color, 1.0f);
}