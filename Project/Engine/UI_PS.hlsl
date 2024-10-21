#include "Value.hlsli"
#include "Func.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;

};

#define Altas_Tex   Tex_0
#define LeftTop     Vec2_0
#define Size        Vec2_1
#define Alpha       Float_0

float4 main(VSOut _in) : SV_TARGET
{
    float4 out_color = (float4) 0.0f;
    
    float2 uv = _in.uv * Size + LeftTop;
    out_color = Altas_Tex.Sample(Sampler_Anisotropic, uv);
    
    if(out_color.a == 0.0f)
        discard;
    
    if (Alpha != 0.0f)
        out_color.a *= Alpha;
    
    return out_color;
}