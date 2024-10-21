#include "Value.hlsli"
#include "Func.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 weights : BLENDWEIGHT;
    float4 indices : BLENDINDICES;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    
    float3 view_pos : POSITION;
    
    float3 view_normal : NORMAL;
    float3 view_tangent : TANGENT;
    float3 view_binormal : BINORMAL;

};


VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.0f;
    
    if (g_iAnim)
    {
        //_in.pos = 0.0f;
        Skinning(_in.pos, _in.tangent, _in.binormal, _in.normal, _in.weights, _in.indices, 0);
    }
    
    //if (Socket_Index != -1)
    //{
    //    matrix matBone = GetBoneMat((int) Socket_Index, 0);
        
    //    _in.pos = mul(float4(_in.pos, 1.f), matBone);
    //    _in.tangent = mul(float4(_in.tangent, 0.f), matBone);
    //    _in.binormal = mul(float4(_in.binormal, 0.f), matBone);
    //    _in.normal = mul(float4(_in.normal, 0.f), matBone);
    //}
    
    output.view_pos = mul(float4(_in.pos, 1.0f), Mat_WV).xyz;
    output.view_normal = normalize(mul(float4(_in.normal, 0.f), Mat_WV)).xyz;
    output.view_tangent = normalize(mul(float4(_in.tangent, 0.f), Mat_WV)).xyz;
    output.view_binormal = normalize(mul(float4(_in.binormal, 0.f), Mat_WV)).xyz;
    //   
    output.color = _in.color;
    //output.pos = mul(float4(_in.pos, 1.f), a);
    output.pos = mul(float4(_in.pos, 1.f), Mat_WVP);
    //output.pos = float4(_in.pos, 1.f);
    output.uv = _in.uv;
    
    return output;
}