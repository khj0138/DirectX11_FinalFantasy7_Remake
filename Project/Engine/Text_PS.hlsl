#include "Value.hlsli"
#include "Func.hlsli"

struct GSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

#define Altas_Tex       Tex_0
#define Alpha           Float_0
#define Color           Vec4_0
#define Damage          Int_0

float3 ConvertSRGBToLinear(float3 srgbIn)
{
    float3 linearOut;
    linearOut.r = (srgbIn.r <= 0.04045) ? srgbIn.r / 12.92 : pow((srgbIn.r + 0.055) / 1.055, 2.4);
    linearOut.g = (srgbIn.g <= 0.04045) ? srgbIn.g / 12.92 : pow((srgbIn.g + 0.055) / 1.055, 2.4);
    linearOut.b = (srgbIn.b <= 0.04045) ? srgbIn.b / 12.92 : pow((srgbIn.b + 0.055) / 1.055, 2.4);
    return linearOut;
}

float4 main(GSOut _in) : SV_TARGET
{
    float4 out_color = (float4) 0.0f;
    
    out_color = Altas_Tex.Sample(Sampler_Anisotropic, _in.uv);

    if (Damage == 1)
    {
        out_color.a *= Alpha;
    }
    else
    {
        if (out_color.b >= 0.0f && out_color.r == 0.0f && out_color.g == 0.0f)
        {
            discard;
        }
        else
        {
            float a = max(max(out_color.r, out_color.g), out_color.b);
        
            if (a < 0.5f)
            {
                discard;
            }
            out_color.r = a;
            out_color.b = a;
            out_color.g = a;
            
            if (Color.a > 0.0f)
            {
                out_color.rgb *= Color.rgb;
            }
            if (Alpha > 0.0f)
            {
                out_color.a *= Alpha;
            }
        }
    }
    
    return out_color;
}   