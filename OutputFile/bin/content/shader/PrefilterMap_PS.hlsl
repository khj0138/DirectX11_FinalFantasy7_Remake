#include "BRDF.hlsli"

struct VSOut
{
    float4 pos : SV_POSITION;
    float3 dir : TEXCOORD0;
};

#define EnvironmentMap  Cube_0  
#define Roughness       Float_0

float3 PrefilterEnvMap(float Roughness, float3 WorldDir);
const uint Mip_Levels = 6; // 생성할 Mipmap 레벨 수

float4 main(VSOut _in) : SV_TARGET
{
    float3 color = PrefilterEnvMap(Roughness, _in.dir);
    return float4(color, 1.0);
}

float3 PrefilterEnvMap(float _roughness, float3 WorldDir)
{
    float3 PrefilteredColor = 0;
    const uint NumSamples = 1024;
    float TotalWeight = 0;

    for (uint i = 0; i < NumSamples; ++i)
    {
        float2 Xi = Hammersley(i, NumSamples);
        float3 H = ImportanceSampleGGX(Xi, _roughness, WorldDir);
        float3 L = 2 * dot(WorldDir, H) * H - WorldDir;
        float NoL = saturate(dot(WorldDir, L));

        if (NoL > 0)
        {
            PrefilteredColor += EnvironmentMap.SampleLevel(Sampler_Min_Mag_Linear, L, _roughness * 6.0f).rgb * NoL;
            TotalWeight += NoL;
        }
    }
    

    return PrefilteredColor / TotalWeight;
}