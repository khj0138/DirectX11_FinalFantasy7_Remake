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

    uint inst_id : SV_InstanceID;
    
    row_major matrix mat_world : WORLD;
    row_major matrix mat_wv : WV;
    row_major matrix mat_wvp : WVP;
    uint row_idx : ROWINDEX;
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
        Skinning(_in.pos, _in.tangent, _in.binormal, _in.normal, _in.weights, _in.indices, _in.row_idx);
    }
    
    output.view_pos = mul(float4(_in.pos, 1.0f), _in.mat_wv).xyz;
    output.view_normal = normalize(mul(float4(_in.normal, 0.f), _in.mat_wv)).xyz;
    output.view_tangent = normalize(mul(float4(_in.tangent, 0.f), _in.mat_wv)).xyz;
    output.view_binormal = normalize(mul(float4(_in.binormal, 0.f), _in.mat_wv)).xyz;
    //   
    output.color = _in.color;
    output.pos = mul(float4(_in.pos, 1.f), _in.mat_wvp);
    output.uv = _in.uv;
    
    return output;
}