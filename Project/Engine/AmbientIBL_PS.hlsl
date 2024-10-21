#include "Value.hlsli"
#include "BRDF.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
};

#define Color_Metalic           Tex_0
#define Normal_Roughness        Tex_1
#define Position                Tex_2
#define Emissive_occlusion      Tex_3
#define BRDF                    Tex_5

#define Prefiltermap_Array      Cube_Array
#define IrradianceMap_Array     Tex_Array

float3 ApproximateSpecularIBL(float3 SpecularColor, float roughness, float3 N, float3 V, int index);

float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (float3(1.0f, 1.0f, 1.0f) - F0) * pow(1.0 - cosTheta, 5.0);
}

int FindNearCaptureObject(float3 pos)
{
    for (uint i = min_level; i <= max_level; i++)
    {
        for (uint j = 0; j < object_cnt; j++)
        {
            if (Captures[j].level == i)
            {
                float dis = distance(Captures[j].world_pos.xyz, pos);

                if (dis <= Captures[j].radius)
                {
                    return j;
                }

            }
        }
    }
    return -1;
}

float4 main(VSOut _in) : SV_TARGET
{
    float2 screen_uv = _in.pos.xy / Resolution.xy;
            
    float3 color = Color_Metalic.Sample(Sampler_Anisotropic, screen_uv).rgb;
    float3 obj_pos = Position.Sample(Sampler_Anisotropic, screen_uv).rgb;
    
    //float3 diffuse = color / PI;

    float metalic = Color_Metalic.Sample(Sampler_Anisotropic, screen_uv).a;
 
    //if (Metalic)
    
    float roughness = Normal_Roughness.Sample(Sampler_Anisotropic, screen_uv).a;
    float occlusion = Emissive_occlusion.Sample(Sampler_Anisotropic, screen_uv).a;
    
    float3 diffuse = color; //- color * metalic;
    //diffuse /= PI;
    float3 world_pos = mul(float4(obj_pos, 1.0f), Mat_View_Inv);
    int index = FindNearCaptureObject(world_pos);
    
    
    float3 N = Normal_Roughness.Sample(Sampler_Anisotropic, screen_uv).rgb;
    float3 V = normalize(-obj_pos); // 뷰어 방향 벡터
    
    float3 wN = normalize(mul(float4(N, 0.0f), Mat_View_Inv));
    float3 wV = normalize(mul(float4(V, 0.0f), Mat_View_Inv));
    
    // 언리얼에서 첫번째 인자로 (0.08 * Specular.xxx) 이게 들어간다고 하는거 같다. 그리고 Specular의 기본값이 0.5인 것 같다.
    float3 F0 = lerp(0.04f, color, metalic.xxx);
    
     
    float dotNV = saturate(dot(wN, wV));
    
    float2 uv = DirectionToSpherical(wN);
    
    float3 irradiance;
    
    if (index == -1)
    {
        discard;
    }
    else
    {
        irradiance = IrradianceMap_Array.Sample(Sampler_Linear, float3(uv, index));
    }
    
    float3 specular = ApproximateSpecularIBL(F0, roughness, wN, wV, index);
   
    float3 kS = F0 + (max((1.0f - roughness).xxx, F0) - F0) * pow(1.0 - saturate(dot(N, V)), 5.0);
    //float3 kS = F0;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    
    kD *= 1.0f - metalic;
    
    // ===== TEST ======
    //float3 F = fresnelSchlick(F0, saturate(dot(N, V)));
    //float3 kD = lerp(float3(1.0f, 1.0f, 1.0f) - F, float3(0.0f, 0.0f, 0.0f), metalic.xxx);
    // =================
    
    diffuse = diffuse * irradiance;
    
    //float3 final_color = (diffuse * ((1.0f - spec_color) * (1.0f - metalic)) + specular) * light_info.color.rgb;
    float3 final_color = (kD * diffuse) + specular;
    final_color = final_color * occlusion;
    
    //float3 final_color = diffuse * (1.0f - metalic) + specular * light_info.color.rgb;
    return float4(final_color, 1.0f);

}


float3 ApproximateSpecularIBL(float3 SpecularColor, float roughness, float3 N, float3 V, int index)
{
    float NoV = saturate(dot(N, V));
    //float3 R = 2 * dot(V, N) * N - V;
    float3 R = reflect(-V, N);
    
    
    float mipLevel = roughness * (6.0f); // MaxMipLevels는 큐브맵의 최대 Mipmap 레벨 수
    float3 PrefilteredColor;
    
    PrefilteredColor = Prefiltermap_Array.SampleLevel(Sampler_Min_Mag_Linear, float4(R, index), mipLevel);
    //float3 rgb = Texxx.SampleLevel(Sampler_Min_Mag_Linear, R, mipLevel).rgb;
    //PrefilterMap.SampleGrad()
    //float3 PrefilteredColor = PrefilterMap.Sample(Sampler_0, R);
    
    float4 EnvBRDF = BRDF.Sample(Sampler_Point, float2(NoV, 1.0f - roughness));
    return PrefilteredColor * (SpecularColor * EnvBRDF.x + EnvBRDF.y);
}