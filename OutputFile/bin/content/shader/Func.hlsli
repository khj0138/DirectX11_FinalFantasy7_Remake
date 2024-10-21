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
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV �� ���
    vUV.y -= (sin((_NomalizedThreadID - (Acc_Time /*�׷��� ���� �̵� �ӵ�*/)) * 2.f * 3.1415926535f * 10.f /*�ݺ��ֱ�*/) / 2.f);
    
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


// ACES filmic tone mapping ���
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
        
    // DirLight �� ���
    if (0 == light_info.light_type)
    {
        // ViewSpace ������ ������ ����
        view_light_dir = normalize(mul(float4(normalize(light_info.world_dir.xyz), 0.f), Mat_View)).xyz;
        
        // ViewSpace ������ �븻���Ϳ� ������ ������ ���� (����Ʈ �ڻ��� ��Ģ)    
        light_pow = saturate(dot(_view_normal, -view_light_dir));
        
        // �ݻ籤
        float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        float3 eye = normalize(_view_pos);
        
        // �ݻ籤�� ���� ���ϱ�
        spec_pow = saturate(dot(view_reflect, -eye));
        spec_pow = pow(spec_pow, 20);
    }
    
    // Point Light �� ���
    else if (1 == light_info.light_type)
    {
        float dist_pow = 1.f;
        
        // ViewSpace ������ ������ ��ġ
        float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        
        // �������κ��� ���� ���� ���� ���ϱ�
        view_light_dir = normalize(_view_pos - view_light_pos);
        
        // ����Ʈ ����Ʈ�κ��� �Ÿ�üũ
        float dist = distance(_view_pos, view_light_pos);
        dist_pow = saturate(1.f - (dist / light_info.attenuation_radius));
               
        // ViewSpace ������ �븻���Ϳ� ������ ������ ���� (����Ʈ �ڻ��� ��Ģ)    
        light_pow = saturate(dot(_view_normal, -view_light_dir)) * dist_pow;
        
        // �ݻ籤
        float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        float3 eye = normalize(_view_pos);
        //reflect()
        // �ݻ籤�� ���� ���ϱ�
        spec_pow = saturate(dot(view_reflect, -eye));
        spec_pow = pow(spec_pow, 20) * dist_pow;
    }
    else
    {
        //�Ÿ��� ���� �� ����
        float dist_pow = 1.f;
        
        //// ���� �⺻ ����
        //float3 light_dir = normalize(mul(float4(light_info.world_dir.xyz, 1.0f), Mat_View)).xyz;
        
        //// ���� ������Ʈ ������ ����
        //float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        //view_light_dir = normalize(_view_pos - view_light_pos);
        
        //// ������ ����մϴ�.
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

        // ���� ��ġ (�� ����)
        float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        view_light_dir = normalize(view_light_pos - _view_pos);

        // ������ ����Ͽ� ���� ������ ��
        float cosAngle = dot(light_dir, -view_light_dir); // ���� ����� �ݴ� �������� ����
        if (cosAngle < cos(light_info.angle))
        {
            light_pow = 0; // ���� ������Ʈ�� �������� ����
        }
        else
        {
            // ���� ������Ʈ ǥ�� �븻 ���� ���� ���
            float ndotl = saturate(dot(_view_normal, view_light_dir));
            if (ndotl > 0)
            {
                light_pow = ndotl; // ���� ������Ʈ �ո鿡 ����
            }
            else
            {
                light_pow = 0; // ���� ������Ʈ �޸鿡 �������� ����
            }
        }

        // �Ÿ��� ���� ���� ���
        //float dist = distance(_view_pos, view_light_pos);
        //dist_pow = saturate(1.f - (dist / light_info.radius));

        float dist = distance(_view_pos, view_light_pos);
        dist_pow = saturate(cos((dist / light_info.attenuation_radius) * (PI / 2.f)));
        
        // �ݻ� ���� ���
        float3 view_reflect = normalize(reflect(-view_light_dir, _view_normal));
        float3 eye = normalize(-_view_pos); // ����� ����

        // Specular ���
        spec_pow = saturate(dot(view_reflect, eye));
        spec_pow = pow(spec_pow, 20) * dist_pow;
        
        light_pow = saturate(light_pow * dist_pow);

    }
  
    
    // ��� �����ϱ�
    //_light_color.diffuse += light_info.color * light_pow;
    //_light_color.ambient += light_info.color * light_info.ambient_coeff;
    //_light_color.specular += light_info.color * spec_pow;
    
    _light_color += light_info.color * light_pow;
    //_spec_pow += spec_pow;
}
#endif