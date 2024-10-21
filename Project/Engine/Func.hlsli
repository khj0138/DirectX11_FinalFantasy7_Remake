#ifndef _FUNC
#define _FUNC

#include "Value.hlsli"
#include "Struct.hlsli"

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += Acc_Time * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (Acc_Time /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += _NoiseTex[pixel + offset + int2(j, i)] * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return JointMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}


// ACES filmic tone mapping 사용
float3 ToneMapping(float3 hdr_color)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    
    return saturate((hdr_color * (a * hdr_color + b)) / (hdr_color * (c * hdr_color + d) + e));
}


void CalcLight3D(float3 _view_pos, float3 _view_normal, int _light_idx, inout float4 _light_color, inout float _spec_pow)
{
    LightInfo light_info = Lights[_light_idx];
    
    float light_pow = 0.f;
    float spec_pow = 0.f;
    float3 view_light_dir = (float3) 0.f;
        
    // DirLight 인 경우
    if (0 == light_info.light_type)
    {
        // ViewSpace 에서의 광원의 방향
        view_light_dir = normalize(mul(float4(normalize(light_info.world_dir.xyz), 0.f), Mat_View)).xyz;
        
        // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)    
        light_pow = saturate(dot(_view_normal, -view_light_dir));
        
        // 반사광
        float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        float3 eye = normalize(_view_pos);
        
        // 반사광의 세기 구하기
        spec_pow = saturate(dot(view_reflect, -eye));
        spec_pow = pow(spec_pow, 20);
    }
    
    // Point Light 인 경우
    else if (1 == light_info.light_type)
    {
        float dist_pow = 1.f;
        
        // ViewSpace 에서의 광원의 위치
        float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        
        // 광원으로부터 오는 빛의 방향 구하기
        view_light_dir = normalize(_view_pos - view_light_pos);
        
        // 포인트 라이트로부터 거리체크
        float dist = distance(_view_pos, view_light_pos);
        dist_pow = saturate(1.f - (dist / light_info.attenuation_radius));
               
        // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)    
        light_pow = saturate(dot(_view_normal, -view_light_dir)) * dist_pow;
        
        // 반사광
        float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        float3 eye = normalize(_view_pos);
        //reflect()
        // 반사광의 세기 구하기
        spec_pow = saturate(dot(view_reflect, -eye));
        spec_pow = pow(spec_pow, 20) * dist_pow;
    }
    else
    {
        //거리에 따른 빛 세기
        float dist_pow = 1.f;
        
        //// 빛의 기본 방향
        //float3 light_dir = normalize(mul(float4(light_info.world_dir.xyz, 1.0f), Mat_View)).xyz;
        
        //// 빛과 오브젝트 사이의 방향
        //float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        //view_light_dir = normalize(_view_pos - view_light_pos);
        
        //// 내적을 계산합니다.
        //float angle = acos(dot(light_dir, view_light_dir));
        
        //if(angle < light_info.angle)
        //    light_pow = 1;
        
        //float dist = distance(_view_pos, view_light_pos);
        //dist_pow = saturate(1.f - (dist / light_info.radius));
        
        //float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        //float3 eye = normalize(_view_pos);
        
        //spec_pow = saturate(dot(view_reflect, -eye));
        //spec_pow = pow(spec_pow, 20) * dist_pow;
        
        
        float3 light_dir = normalize(mul(float4(light_info.world_dir.xyz, 0.0f), Mat_View)).xyz;

        // 빛의 위치 (뷰 공간)
        float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        view_light_dir = normalize(view_light_pos - _view_pos);

        // 내적을 계산하여 빛의 각도와 비교
        float cosAngle = dot(light_dir, -view_light_dir); // 빛의 방향과 반대 방향으로 내적
        if (cosAngle < cos(light_info.angle))
        {
            light_pow = 0; // 빛이 오브젝트에 도달하지 않음
        }
        else
        {
            // 빛과 오브젝트 표면 노말 간의 관계 계산
            float ndotl = saturate(dot(_view_normal, view_light_dir));
            if (ndotl > 0)
            {
                light_pow = ndotl; // 빛이 오브젝트 앞면에 도달
            }
            else
            {
                light_pow = 0; // 빛이 오브젝트 뒷면에 도달하지 않음
            }
        }

        // 거리에 따른 감쇠 계산
        //float dist = distance(_view_pos, view_light_pos);
        //dist_pow = saturate(1.f - (dist / light_info.radius));

        float dist = distance(_view_pos, view_light_pos);
        dist_pow = saturate(cos((dist / light_info.attenuation_radius) * (PI / 2.f)));
        
        // 반사 벡터 계산
        float3 view_reflect = normalize(reflect(-view_light_dir, _view_normal));
        float3 eye = normalize(-_view_pos); // 뷰어의 방향

        // Specular 계산
        spec_pow = saturate(dot(view_reflect, eye));
        spec_pow = pow(spec_pow, 20) * dist_pow;
        
        light_pow = saturate(light_pow * dist_pow);

    }
  
    
    // 결과 전달하기
    //_light_color.diffuse += light_info.color * light_pow;
    //_light_color.ambient += light_info.color * light_info.ambient_coeff;
    //_light_color.specular += light_info.color * spec_pow;
    
    _light_color += light_info.color * light_pow;
    //_spec_pow += spec_pow;
}
#endif