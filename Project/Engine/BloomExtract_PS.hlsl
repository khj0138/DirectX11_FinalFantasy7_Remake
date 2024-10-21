#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

#define HDR Tex_0

float4 main(VSOut _in) : SV_TARGET
{
    const float4 color = HDR.Sample(Sampler_Point, _in.uv);
	
    const float BrightnessUpperThreshold = 6.0f;
    float BrightnessThreshold = 1.0f;
    const float brightness = dot(float3(0.2126, 0.7152, 0.0722), color.xyz); // luma conversion

    float4 output;
    
    output = color;

    if (brightness > BrightnessThreshold /*&& brightness < BrightnessUpperThreshold*/)
        output = color;
    else
        output = float4(0, 0, 0, 1);
    
    output.a = 1.0f;
    return output;
}