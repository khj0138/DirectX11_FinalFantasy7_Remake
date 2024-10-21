#include "Value.hlsli"

struct GSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    uint id : SV_InstanceID;
};

StructuredBuffer<Particle> ParticleBuffer : register(t20);

#define Intensity Float_0

float4 main(GSOut _in) : SV_TARGET
{   
    float4 output = float4(1.f, 0.f, 1.f, 1.f);
    
    if (BTex_0)
    {
        output = Tex_0.Sample(Sampler_Anisotropic, _in.uv);
        //output.a = (output.r + output.g + output.b) / 3.0f;
        output.rgb *= ParticleBuffer[_in.id].color.rgb;
        output.rgb *= Intensity;
        //output.a = 1.0f;
    }
    else
    {
        output.rgb = ParticleBuffer[_in.id].color.rgb;
        output.a = 1.0f;

    }
    
    if (BTex_1)
    {
       //output.rgb *= Tex_1.Sample(Sampler_Anisotropic, _in.uv).rgb;
       // output.rgb *= Tex_1.Sample(Sampler_Anisotropic, ParticleBuffer[_in.id].test_uv).rgb;
       // output.rgb = Tex_1.Sample(Sampler_Anisotropic, _in.uv).rgb;
       // output.a *= Tex_1.Sample(Sampler_Anisotropic, _in.uv).r;
        //output.rgb = Tex_0.Sample(Sampler_Anisotropic, _in.uv).rgb;
       // output.a = (output.r + output.g + output.b) / 3.0f;

    }

    
    return output;
}