#include "Value.hlsli"

struct VSIn
{
    float3 pos          : POSITION;
    float2 uv           : TEXCOORD;
    
    float3 normal       : NORMAL;
    float3 tangent      : TANGENT;
    float3 binormal     : BINORMAL;
};

struct VSOut
{
    float4 pos              : SV_Position;
    float2 uv               : TEXCOORD;
        
    float3 view_pos         : POSITION;
    
    float3 view_normal      : NORMAL;
    float3 view_tangent     : TANGENT;
    float3 view_binormal    : BINORMAL;
};

VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.f;
        
    // 로컬에서의 Normal 방향을 월드로 이동      
    output.view_pos = mul(float4(_in.pos, 1.f), Mat_WV);
    
    output.view_normal = normalize(mul(float4(_in.normal, 0.f), Mat_WV)).xyz;
    output.view_tangent = normalize(mul(float4(_in.tangent, 0.f), Mat_WV)).xyz;
    output.view_binormal = normalize(mul(float4(_in.binormal, 0.f), Mat_WV)).xyz;
       
        
    output.pos = mul(float4(_in.pos, 1.f), Mat_WVP);
    output.uv = _in.uv;
      
    return output;
}