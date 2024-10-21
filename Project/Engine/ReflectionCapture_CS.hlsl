#include "Value.hlsli"
#include "BRDF.hlsli"

//#define Color_Metalic       Tex_0
//#define Normal_Roughness    Tex_1
//#define Position            Tex_2
//#define BRDF                Tex_5
//#define Irradiance          Tex_6

//#define IrradianceMap       Arr_Tex_0
//#define PrefilterMap        Arr_Cube_0

//RWTexture2D<float4> EnvironmentReflection : register(u0);


//struct CaptureObjectInfo
//{
//    float3 world_pos;
//    float radius;
//    uint level;
//};

//StructuredBuffer<CaptureObjectInfo> CaptureObjects : register(t16);

//float3 ApproximateSpecularIBL(float3 SpecularColor, float roughness, float3 N, float3 V, int index);

//float3 fresnelSchlick(float3 F0, float cosTheta)
//{
//    return F0 + (float3(1.0f, 1.0f, 1.0f) - F0) * pow(1.0 - cosTheta, 5.0);
//}

//int FindNearCaptureObject(float3 pos)
//{
//    for (uint i = min_level; i <= max_level; i++)
//    {
//        for (uint j = 0; j < object_cnt; j++)
//        {
//            if (CaptureObjects[j].level == i)
//            {
//                float dis = distance(CaptureObjects[j].world_pos, pos);

//                if (dis <= CaptureObjects[j].radius)
//                {
//                    return j;
//                }

//            }
//        }
//    }
//    return -1;
//}

[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //float2 uv = float2(DTid.x / 1280.f, DTid.y / 768.0f);
    
    //float3 color = Color_Metalic.SampleLevel(Sampler_Anisotropic, uv, 0).rgb;
    //float3 pos = Position.SampleLevel(Sampler_Anisotropic, uv, 0).rgb;
    //float metalic = Color_Metalic.SampleLevel(Sampler_Anisotropic, uv, 0).a;
    //float roughness = Normal_Roughness.SampleLevel(Sampler_Anisotropic, uv, 0).a;
    
    //float3 world_pos = mul(float4(pos, 1.0f), Mat_View_Inv);
    
    //int index = FindNearCaptureObject(world_pos);
    
    //if (index == -1)
    //    return;
    
    //float3 N = Normal_Roughness.SampleLevel(Sampler_Anisotropic, uv, 0).rgb;
    //float3 V = normalize(-pos);
    //float3 wN = normalize(mul(float4(N, 0.0f), Mat_View_Inv));
    //float3 wV = normalize(mul(float4(V, 0.0f), Mat_View_Inv));
    
    //float3 diffuse = color;
    
    //float3 F0 = lerp(0.04f, color, metalic.xxx);
    
     
    //float NoV = saturate(dot(wN, wV));
    
    //float2 spherical_uv = DirectionToSpherical(wN);
    //float3 irradiance = IrradianceMap.SampleLevel(Sampler_Anisotropic, float3(spherical_uv, index), 0).rgb;
    //float3 specular = ApproximateSpecularIBL(F0, roughness, wN, wV, index);
   
    //float3 kS = specular;
    
    ////float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    ////
    ////kD *= 1.0f - metalic;
    
    //// ===== TEST ======
    //float3 F = fresnelSchlick(F0, saturate(dot(N, V)));
    //float3 kD = lerp(float3(1.0f, 1.0f, 1.0f) - F, float3(0.0f, 0.0f, 0.0f), metalic.xxx);
    //// =================
    
    //diffuse = diffuse * irradiance;
    
    //float4 final_color = float4((kD * diffuse) + specular, 1.0f);
    //EnvironmentReflection[DTid.xy] = final_color;
}


//float3 ApproximateSpecularIBL(float3 SpecularColor, float roughness, float3 N, float3 V, int index)
//{
//    float NoV = saturate(dot(N, V));
//    float3 R = reflect(-V, N);
    
//    float mipLevel = roughness * (6.0f);
//    float3 PrefilteredColor = PrefilterMap.SampleLevel(Sampler_Min_Mag_Linear, float4(R, index), mipLevel);
    
//    float4 EnvBRDF = BRDF.SampleLevel(Sampler_Anisotropic, float2(NoV, 1.0f - roughness), 0);
//    return PrefilteredColor * (SpecularColor * EnvBRDF.x + EnvBRDF.y);
//}