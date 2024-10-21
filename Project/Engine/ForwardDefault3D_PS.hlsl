#include "Value.hlsli"
#include "Func.hlsli"

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

#define Color       Tex_0
#define Normal      Tex_1

#define Emissive_Factor Float_0
#define Emissive_Color  Vec4_0

float4 main(VSOut _in) : SV_Target
{
    float4 out_color = (float4) 0.f;
    
    float3 view_normal = _in.view_normal;
    
    if (BTex_0)
    {
        out_color.rgb = Color.Sample(Sampler_Anisotropic, _in.uv).rgb;
        out_color.a = out_color.r;
    }
    
    if (out_color.a < 0.1f)
    {
        discard;
    }
    
    if (BTex_1)
    {
        float3 normal = Normal.Sample(Sampler_Linear, _in.uv).rgb;
        normal = normal * 2.0f - 1.0f;
        
        float3x3 mat_rotate = { _in.view_tangent, -_in.view_binormal, _in.view_normal };
        view_normal = mul(normal, mat_rotate);
    }
    
    out_color.rgb = pow(out_color.rgb, 2.2f);
    
    return out_color;
}