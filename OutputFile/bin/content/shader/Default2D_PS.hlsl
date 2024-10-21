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

#define Altas_Tex       Tex_0
#define Emissive_Tex    Tex_1
#define Alpha_Tex       Tex_2
#define Normal_Tex      Tex_3

#define bAnimUse        Int_0
#define LeftTop         Vec2_0
#define Slice           Vec2_1
#define Offset          Vec2_2
#define BackSize        Vec2_3
#define Color           Vec4_0

float4 main(VSOut _in) : SV_TARGET
{
    float4 out_color = (float4) 0.0f;
    float3 emissive = (float3) 0.0f;
    float3 view_normal = _in.view_normal;
    
    if (BTex_0)
    {
        if (bAnimUse)
        {
            float2 uv = LeftTop + (BackSize * _in.uv);
            uv -= ((BackSize - Slice) / 2.f);
            uv -= Offset;
            
            if (LeftTop.x < uv.x && uv.x < LeftTop.x + Slice.x
                && LeftTop.y < uv.y && uv.y < LeftTop.y + Slice.y)
            {
                out_color.rgb = Altas_Tex.Sample(Sampler_Anisotropic, uv).rgb;
                out_color.a = out_color.r;
            }
            else
            {
                out_color = float4(1.f, 1.f, 0.f, 1.f);
            }
            
            if (out_color.a <= 0.01f)
            {
                discard;
            }
        }
        else
        {
            out_color = Altas_Tex.Sample(Sampler_Anisotropic, _in.uv);
        }
    }
    else
    {
        out_color = float4(1.f, 1.f, 1.f, 1.f);
    }
    
    float2 uv = LeftTop + (BackSize * _in.uv);
    uv -= ((BackSize - Slice) / 2.f);
    uv -= Offset;
    
    if (BTex_1)
    {
        emissive = Emissive_Tex.Sample(Sampler_Anisotropic, uv).rgb;
    }
    
    if (BTex_2)
    {
        out_color.a = Alpha_Tex.Sample(Sampler_Anisotropic, uv).r;
    }
    
    if (BTex_3)
    {        
        float3 normal = Normal_Tex.Sample(Sampler_Anisotropic, uv).xyz;          
        normal = normal * 2.f - 1.f;
        
        float3x3 mat_rotate = { _in.view_tangent, -_in.view_binormal, _in.view_normal };     
        view_normal = normalize(mul(normal, mat_rotate));
    }
    //
    //
    //float4 light_color = (float4) 0.0f;
    //float spec_pow = 0.0f;
    
    //for (int i = 0; i < 2; ++i)
    //{
    //    CalcLight3    D(_in.view_pos, view_normal, i, light_color, spec_pow);
    //}
    
    //out_color.rgb = out_color.rgb * light_color.rgb;
    //out_color.rgb += emissive * 10.f;
    //
    //out_color *= float4(0.51f, 0.0f, 1.0f, 1.f);
    
    if (Color.a != 0.0f)
    {
        
    out_color.rgb *= Color.rgb;
    }
    out_color.rgb += emissive;
    
    return out_color;
}