#include "Value.hlsli"
#include "Func.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

#define HDR Tex_0
#define Bloom Tex_1

float4 main(VSOut _in) : SV_TARGET
{
    float4 output = (float4) 0.0f;
    
    float3 hdr_color = HDR.Sample(Sampler_Point, _in.uv).rgb;
    //float3 bloom_color = Bloom.Sample(Sampler_Linear, _in.uv).rgb;
    
    //float3 sdr_color = ToneMapping(hdr_color + bloom_color);
    
    //output.rgb = pow(sdr_color, 1.0f / 2.2f);
    
    float4 bloom_color = Bloom.Sample(Sampler_Linear, _in.uv);
    
    float3 sdr_color = ToneMapping(hdr_color + bloom_color.rgb * bloom_color.a);
    
    output.rgb = pow(sdr_color, 1.0f / 2.2f);
    
    return float4(output.rgb, 1.0f);
}