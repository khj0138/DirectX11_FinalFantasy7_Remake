#ifndef _BRDF
#define _BRDF

#include "Value.hlsli"

// NDF GGX (Trowbridge-Reitz) 모델 사용
float DistributionFunc(float roughness, float3 n, float3 h)
{
    // Normal Distribution Function 노말 분포 함수 (확률 밀도 함수의 일종 PDF)
    // D(h) = alpha ^ 2 / (PI * ((dot(n, h) ^ 2) * ((alpha ^ 2 - 1) + 1)) ^ 2)
    
    float NoH = saturate(dot(n, h));
    float a = roughness * roughness;
    float a2 = a * a;
    
    //float roughness2 = roughness * roughness;
    float denominator = (NoH * NoH * (a2 - 1) + 1);

    return a2 / max(EPSILON, (PI * denominator * denominator));
}

// Fresnel Schlick 근사 모델 사용
float3 FresnelFunc(float3 v, float3 h, float3 F0)
{
    // F(v,h) = F0 + (1 - F0) * (1 - dot(v,h)) ^ 5
    // F0 = specular_color
    
    float VoH = saturate(dot(v, h));
    
    return F0 + (1.0f - F0) * pow(2, (-5.55473 * VoH - 6.98316) * VoH); // test
    
   // return F0 + (1.0f - F0) * pow((1 - VoH), 5);
}

// Schlick-GGX 근사 모델 사용
float Geometry(float3 n, float3 v, float k)
{
    float NoV = saturate(dot(n, v));
 
    return NoV / (NoV * (1.0f - k) + k);

}

float GeometryFunc(float3 n, float3 v, float3 l, float k)
{
    // G(l,v,h) = G1(v) * G1(l)
    // G1(v) = dot(n,v) / (dot(n,v) * (1 - k) + k)
    
    k = pow(k + 1, 2) / 8.0f; // test
    return Geometry(n, v, k) * Geometry(n, l, k);
}

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // 1/2^32
}

float2 Hammersley(uint i, uint N) // vs halton sequence
{
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

float3 ImportanceSampleGGX(float2 Xi, float Roughness, float3 N)
{
    float a = Roughness * Roughness;
    float Phi = 2 * PI * Xi.x;
    float CosTheta = sqrt((1 - Xi.y) / (1 + (a * a - 1) * Xi.y));
    float SinTheta = sqrt(1 - CosTheta * CosTheta);
    float3 H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;
    float3 UpVector = abs(N.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    float3 TangentX = normalize(cross(UpVector, N));
    float3 TangentY = cross(N, TangentX);

    return normalize(TangentX * H.x + TangentY * H.y + N * H.z); // normalize 추가
}

float3 SphericalToDirection(float2 uv)
{
    float theta = (-uv.x + 0.5f) * 2.0f * PI; // U를 각도로 변환
    float phi = (-uv.y + 0.5f) * PI; // V를 각도로 변환

    float x = cos(phi) * cos(theta); // 방향 벡터 계산
    float y = sin(phi);
    float z = cos(phi) * sin(theta);

    return float3(x, y, z);
}
float2 DirectionToSpherical(float3 dir)
{
    float phi = atan2(dir.y, dir.x);
    float theta = acos(dir.z);
    float s = phi / (2.0 * PI);
    float t = theta / PI;
    return float2(s, t);

}

float2 ConvertCubeMapVec3To2D(float3 dir)
{
    float absX = abs(dir.x);
    float absY = abs(dir.y);
    float absZ = abs(dir.z);

    float u, v;
    float2 uv = float2(0.0, 0.0);

    // X축이 최대인 경우
    if (absX >= absY && absX >= absZ)
    {
        // 오른쪽 또는 왼쪽 면
        float signX = dir.x > 0 ? 1.0 : -1.0;
        u = -dir.z * signX;
        v = -dir.y;
        uv.x = (signX > 0) ? (u + 1.0) / 4.0 + 0.5 : (-u + 3.0) / 4.0; // 오른쪽 또는 왼쪽 면의 U 좌표
        uv.y = (1.0 - v) / 3.0; // Y축은 상하 반전
    }
    // Y축이 최대인 경우
    else if (absY >= absX && absY >= absZ)
    {
        // 위 또는 아래 면
        float signY = dir.y > 0 ? 1.0 : -1.0;
        u = dir.x;
        v = -dir.z * signY;
        uv.x = (u + 1.0) / 4.0 + 0.25; // 위 또는 아래 면의 U 좌표
        uv.y = (signY > 0) ? (-v + 1.0) / 3.0 : (v + 2.0) / 3.0; // 위 또는 아래 면의 V 좌표
    }
    // Z축이 최대인 경우
    else
    {
        // 앞 또는 뒤 면
        float signZ = dir.z > 0 ? 1.0 : -1.0;
        u = dir.x * signZ;
        v = -dir.y;
        uv.x = (signZ > 0) ? (u + 1.0) / 4.0 + 0.25 : (-u + 1.0) / 4.0; // 앞 또는 뒤 면의 U 좌표
        uv.y = (1.0 - v) / 3.0; // Y축은 상하 반전
    }

    return uv;
}

float2 EquirectangularToUV(float3 v)
{
    //float u = atan2(pos.z, pos.x) / (2.0f * PI) + 0.5f;
    //float v = asin(pos.y) / PI + 0.5f;
    //return float2(u, v);
    
    float2 uv = float2(atan2(v.z, v.x), asin(-v.y));
    uv /= float2(-PI * 2.0f, PI);
    uv += float2(0.5, 0.5);
    return uv;
}

#endif