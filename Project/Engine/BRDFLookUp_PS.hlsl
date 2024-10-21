#include "BRDF.hlsli"

struct VSOut
{
    float4 pos : SV_POSITION; // 출력 스크린 공간 위치
    float2 uv : TEXCOORD0; // 출력 UV
};

float2 IntegrateBRDF(float _roughness, float NoV);

float4 main(VSOut _in) : SV_TARGET
{
    float roughness = 1.0f - _in.uv.y;
    float NoV = _in.uv.x;

    // BRDF 계산 로직
    float2 brdfValue = IntegrateBRDF(roughness, NoV);
    
    return float4(brdfValue, 0.0f, 1.0f);
    //float roughness = _in.uv.x;
    //float dotNV = _in.uv.y;

    
    //// BRDF 계산 로직
    //float2 brdfValue = IntegrateBRDF(roughness, dotNV);
    
    //return float4(brdfValue, 0.0f, 1.0f);
}

//float G_Smith(float roughness, float NdotV, float NdotL)
//{
//    float a = roughness * roughness;
//    float a2 = a * a;
//
//    float GGXV = NdotL * sqrt((NdotV - a2 * NdotV) * NdotV + a2);
//    float GGXL = NdotV * sqrt((NdotL - a2 * NdotL) * NdotL + a2);
//
//    return 0.5 / (GGXV + GGXL);
//}

float G_Smith(float roughness, float NdotV, float NdotL)
{
    	// k_direct	 = (roughness + 1)^2 / 8
	// k_IBL	 = roughness^2 / 2
    float k = pow(roughness, 2) / 2.0f;
    
    //float dotNV = saturate(dot(n, v));

    return (max(NdotV, 0.0001f) / (NdotV * (1.0f - k) + k)) * (max(NdotL, 0.0001f) / (NdotL * (1.0f - k) + k));
}


float2 IntegrateBRDF(float _roughness, float NoV)
{
    float3 V;
    float3 N = float3(0.0f, 0.0f, 1.0f); // 나중에 한번 확인
    V.x = sqrt(1.0f - NoV * NoV); // sin(acos(NoV));
    V.y = 0;
    V.z = NoV; //  cos(acos(NoV))
    float A = 0;
    float B = 0;
    const uint NumSamples = 1024;
    for (uint i = 0; i < NumSamples; i++)
    {
        float2 Xi = Hammersley(i, NumSamples);
        float3 H = ImportanceSampleGGX(Xi, _roughness, N);
        float3 L = normalize(2 * dot(V, H) * H - V); // normalize 추가, reflect(-V, H)
        float NoL = saturate(L.z);
        float NoH = saturate(H.z);
        float VoH = saturate(dot(V, H));
        if (NoL > 0)
        {
            float G = G_Smith(_roughness, NoV, NoL);
            float G_Vis = G * VoH / (NoH * NoV);
            float Fc = pow(1 - VoH, 5);
            A += (1.0f - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    return float2(A, B) / NumSamples;
}