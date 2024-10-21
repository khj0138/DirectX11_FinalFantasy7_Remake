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

#define Fill_LT     Vec2_2
#define Fill_RB     Vec2_3

#define Fill_Color  Vec4_0

#define Fill_Amount Float_0

float4 main(VSOut _in) : SV_TARGET
{
    float4 out_color = (float4) 0.0f;
    
    float2 uv = _in.uv * Size + LeftTop;

    out_color = Altas_Tex.Sample(Sampler_Point, uv);
    if (_in.uv.x > Fill_LT.x && _in.uv.x < Fill_RB.x && _in.uv.y > Fill_LT.y && _in.uv.y < Fill_RB.y
        && _in.uv.x < Fill_Amount)
    {
        if (out_color.a == 0.f)
            out_color = Fill_Color;

    }

    
    if (out_color.a == 0.0f)
        discard;
    
    return out_color;
}