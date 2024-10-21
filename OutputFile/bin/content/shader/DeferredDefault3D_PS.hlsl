#include "Value.hlsli"
#include "Func.hlsli"

struct VSOut
{
    float4 pos              : SV_Position;
    float4 color            : COLOR;
    float2 uv               : TEXCOORD;
    
    float3 view_pos         : POSITION;
    float3 view_normal      : NORMAL;
    float3 view_tangent     : TANGENT;
    float3 view_binormal    : BINORMAL;
};

struct PS_OUT
{
    float4 color_metalic        : SV_Target0;
    float4 normal_roughness     : SV_Target1;
    float4 position             : SV_Target2;       
    float4 emissive_occlusion   : SV_Target3;
};

#define Color       Tex_0
#define Normal      Tex_1
#define MRV         Tex_2
#define Occlusion   Tex_3
#define Emissive    Tex_4

#define Emissive_Factor Float_0
#define Emissive_Color  Vec4_0

PS_OUT main(VSOut _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    //output.color_metalic = float4(1.f, 1.f, 1.f, 1.f);
    
    float3 view_normal = _in.view_normal;
    
    if (BTex_0)
    {
        output.color_metalic.rgb = pow(Color.Sample(Sampler_LOD_Linear, _in.uv).rgb, 2.2f); // 선형 색상 공간 변환
    }
    
    if (BTex_1)
    {
        float3 normal = Normal.Sample(Sampler_LOD_Linear, _in.uv).rgb;
        normal = normal * 2.0f - 1.0f;
        
        float3x3 mat_rotate = { _in.view_tangent, -_in.view_binormal, _in.view_normal };
        view_normal = mul(normal, mat_rotate);
    }
    
    if (BTex_2)
    {
        float4 Sample_MRV = MRV.Sample(Sampler_LOD_Linear, _in.uv);
        output.color_metalic.a = Sample_MRV.r;
        output.normal_roughness.a = Sample_MRV.g;
    }
    else
    {
        output.color_metalic.a = 0.0f;
        output.normal_roughness.a = 1.0f;
    }
   
    if (BTex_3)
    {
        output.emissive_occlusion.a = Occlusion.Sample(Sampler_LOD_Linear, _in.uv).r;
    }
     
    if (BTex_4)
    {
        float emissive_factor = Emissive_Factor;
        if (emissive_factor <= 0.0f)
        {
            emissive_factor = 1.0f;
        }
        
        float3 emissive_color = Emissive_Color.rgb;
        if (all(emissive_color == float3(0.0f, 0.0f, 0.0f)))
        {
            emissive_color = float3(1.0f, 1.0f, 1.0f);
        }
        
        output.emissive_occlusion.rgb = Emissive.Sample(Sampler_LOD_Linear, _in.uv).rgb * emissive_color * emissive_factor;
    }
    
    output.normal_roughness.rgb = view_normal;
    output.position = float4(_in.view_pos, 1.f);
    
    return output;
}