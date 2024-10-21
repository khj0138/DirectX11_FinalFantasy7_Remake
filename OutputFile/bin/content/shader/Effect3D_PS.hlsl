#include "Value.hlsli"
#include "Func.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

#define Texture                 Tex_0

#define Emissive_Factor         Float_0

#define UV_X_Boundary           Vec2_0
#define UV_Y_Boundary           Vec2_1
#define UV_Y_Discard_Boundary   Vec2_2
#define Color_Parametr          Vec4_0

float4 main(VSOut _in) : SV_Target
{
    float4 out_color = (float4) 0.f;
    
    if (_in.uv.y < UV_Y_Boundary.x || _in.uv.y > UV_Y_Boundary.y)
    {
        discard;
    }
    else if (_in.uv.x < UV_X_Boundary.x || _in.uv.x > UV_X_Boundary.y)
    {
        discard;
    }
    
    if (_in.uv.y < UV_Y_Discard_Boundary.y && _in.uv.y > UV_Y_Discard_Boundary.x)
    {
        discard;
    }
    
    if (BTex_0)
    {
        out_color.rgb = Texture.Sample(Sampler_Linear, _in.uv).rgb;
        out_color.a = out_color.r;
    }
    else
    {
        out_color.rgba = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    
    if (out_color.a < 0.05f)
    {
        discard;
    }
    

    out_color *= Color_Parametr;
    out_color.rgb = min(out_color.rgb, float3(1.0f, 1.0f, 1.0f));

    if (Emissive_Factor > 0.0f)
    {
        out_color.rgb *= Emissive_Factor;
    }
    
    return out_color;
}