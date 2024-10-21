#include "Value.hlsli"
#include "BRDF.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
};

#define Color_Metalic       Tex_0
#define Normal_Roughness    Tex_1
#define Position            Tex_2
#define Emissive            Tex_3
#define BRDF                Tex_5
#define Irradiance          Tex_6
#define PrefilterMap1       Cube_0
#define Prefiltermap_Array  Cube_Array

inline float AttenuationBRDF(float2 coeffs, float dist)
{
    return 1.0f / (dist * dist);
}

float4 main(VSOut _in) : SV_TARGET
{
    float2 screen_uv = _in.pos.xy / Resolution.xy;
    float3 obj_pos = Position.Sample(Sampler_Point, screen_uv).rgb;
    
    if (all(obj_pos == float3(0.0f, 0.0f, 0.0f)))
    {
        discard;
    }
            
    float3 albedo = Color_Metalic.Sample(Sampler_Point, screen_uv).rgb;
    float3 world_obj_pos = mul(float4(obj_pos, 1.0f), Mat_View_Inv).xyz;

    float metalic = Color_Metalic.Sample(Sampler_Point, screen_uv).a;
    float roughness = Normal_Roughness.Sample(Sampler_Point, screen_uv).a;
    float3 light_color = (float3) 0.0f;
    
    float3 N = Normal_Roughness.Sample(Sampler_Point, screen_uv).rgb;
    float3 V = normalize(-obj_pos); // 뷰어 방향 벡터
    float NoV = saturate(dot(N, V));
        
    float3 emissive = Emissive.Sample(Sampler_Point, screen_uv).rgb;
    float3 diffuse = lerp(albedo, float3(0.0f, 0.0f, 0.0f), metalic.xxx);
    float3 F0 = lerp(0.04f, albedo, metalic.xxx);
 
    for (int i = 0; i < 10; i++)
    {
        LightInfo light_info = Lights[i];
    
        // Directional Light
    
        if (light_info.light_type == 0)
        {
            float3 L = normalize(-mul(float4(normalize(light_info.world_dir.xyz), 0.f), Mat_View).xyz);
            float3 H = normalize(L + V);
            
            float NoL = saturate(dot(N, L));
            
            float distribution = DistributionFunc(roughness, N, H);
            float3 fresnel = FresnelFunc(V, H, F0);
            float geometric = GeometryFunc(N, V, L, roughness);
    
            float denom = max(4.0f * NoL * NoV, 0.0001f);
            float3 specular = (distribution * fresnel * geometric) / denom;
            
            float3 kS = fresnel;
            float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    
            kD *= 1.0f - metalic;
    
            light_color += (kD * albedo / PI + specular) * light_info.intensity * NoL;
        }
        
         // Point Light
        else if (light_info.light_type == 1)
        {
            float3 light_view_pos = mul(float4(normalize(light_info.world_pos.xyz), 0.f), Mat_View).xyz;
            float3 light_dir = normalize(obj_pos - light_view_pos);
        
            //float3 L = normalize(-mul(float4(normalize(light_info.world_dir.xyz), 0.f), Mat_View).xyz);
            float3 L = -light_dir;
            float3 H = normalize(L + V); // 하프 벡터
        
            float NoL = saturate(dot(N, L));
        
            float distance = length(light_info.world_pos.xyz - world_obj_pos);
    
            if (distance < light_info.attenuation_radius)
            {
                float3 radiance = (1.0f - distance / light_info.attenuation_radius) * light_info.color;
                float distribution = DistributionFunc(roughness, N, H);
                float3 fresnel = FresnelFunc(V, H, F0);
                float geometric = GeometryFunc(N, V, L, roughness);
            
                float denom = max(4.0f * NoL * NoV, 0.0001f);
                float3 specular = (distribution * fresnel * geometric) / denom;
            
                float3 kS = fresnel;
                float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    
                kD *= 1.0f - metalic;
    
                light_color += (kD * albedo / PI + specular) * radiance * light_info.intensity * NoL; /** radiance*/;
            }
        }
        else if (light_info.light_type == 2) // 스포트 라이트
        {
            float3 light_view_pos = mul(float4(normalize(light_info.world_pos.xyz), 0.f), Mat_View).xyz;
            float3 light_dir = normalize(obj_pos - light_view_pos);
        
            float3 L = -light_dir;
            float3 H = normalize(L + V); // 하프 벡터
        
            float NoL = saturate(dot(N, L));
        
            float distance = length(light_info.world_pos.xyz - world_obj_pos);
        
            if (distance < light_info.attenuation_radius)
            {
                float3 light_to_obj_dir = normalize(world_obj_pos - light_info.world_pos.xyz); // 빛에서 오브젝트로의 방향
                light_dir = normalize(-light_info.world_dir.xyz); 
                
                float spotEffect = dot(light_to_obj_dir, light_dir);
                float spotCutoff = cos((light_info.angle / 2));
                    
                if (spotEffect > spotCutoff)
                {
                    float3 radiance = (1.0f - distance / light_info.attenuation_radius) * light_info.color;
                    
                    float distribution = DistributionFunc(roughness, N, H);
                    float3 fresnel = FresnelFunc(V, H, F0);
                    float geometric = GeometryFunc(N, V, L, roughness);
        
                    float denom = max(4.0f * NoL * NoV, 0.0001f);
                    float3 specular = (distribution * fresnel * geometric) / denom;
        
                    float3 kS = fresnel;
                    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
                    kD *= 1.0f - metalic;

                    light_color += (kD * albedo / PI + specular) * radiance * light_info.intensity * NoL;
                }
         
            }
        }
   
    }
    
    //light_color += 
    return float4(light_color + albedo * 0.15f + emissive, 1.0f);
    
    //diffuse = diffuse - (diffuse * metalic.xxx);
    //diffuse = diffuse * (float3(1.0f, 1.0f, 1.0f) - spec_color);
    //float3 final_color = (diffuse * ((1.0f - spec_color) * (1.0f - metalic)) + specular) * light_info.color.rgb;
    //float3 final_color = diffuse * (1.0f - metalic) + specular * light_info.color.rgb;
}