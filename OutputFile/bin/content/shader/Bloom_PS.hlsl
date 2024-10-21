#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

#define RenderTarget Tex_0

float4 main(VSOut _in) : SV_TARGET
{
    const float4 color = RenderTarget.Sample(Sampler_Point, _in.uv);
	
	// clamp values that is made very large by NDF in BRDF
	// this reduces bloom flickering
    const float BrightnessUpperThreshold = 6.0f;
    float BrightnessThreshold = 1.0f;
    const float brightness = dot(float3(0.2126, 0.7152, 0.0722), color.xyz); // luma conversion

    float4 output;
    
    output = color;
    if (brightness > BrightnessThreshold && brightness < BrightnessUpperThreshold)
        output = color;
    else
        output = float4(0, 0, 0, 1);
    
    return output;
}