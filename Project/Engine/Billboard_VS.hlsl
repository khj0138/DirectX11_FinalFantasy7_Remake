#include "Value.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
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
    
    float4 positionOS = float4(_in.pos, 1.0f);
    
    float3 pivotPosOS = float3(0, 0, 0);
    float4 pivotPosWS = mul(float4(pivotPosOS.xyz, 1), Mat_World);
    float4 cameraPosWS = float4(Camera_Pos.xyz, 1);
    float3 cameraLookDir = normalize(pivotPosWS.xyz - cameraPosWS.xyz); // 카메라 방향 벡터 계산
                    
                // 카메라 방향 벡터를 회전 행렬로 변환
    float4x4 cameraRotMatrix;
    float3 upVector = float3(0, 1, 0);
    float3 rotM2 = cameraLookDir;
    float3 rotM0 = normalize(cross(upVector, rotM2));
    float3 rotM1 = cross(rotM2, rotM0);
    cameraRotMatrix[0] = float4(rotM0.x, rotM1.x, rotM2.x, 0);
    cameraRotMatrix[1] = float4(rotM0.y, rotM1.y, rotM2.y, 0);
    cameraRotMatrix[2] = float4(rotM0.z, rotM1.z, rotM2.z, 0);
    cameraRotMatrix[3] = float4(0, 0, 0, 1);
                
    positionOS = mul(positionOS, transpose(Mat_World));
    positionOS = mul(cameraRotMatrix, positionOS);
    positionOS = mul(positionOS, transpose(Mat_World_Inv));
                
    float4 view_pos = mul(float4(positionOS.xyz, 1), Mat_WV);
                 
    output.pos      = mul(float4(view_pos.xyz, 1), Mat_Proj);
    output.color    = _in.color;
    output.uv       = _in.uv;
    
    output.view_pos         = view_pos; /*mul(float4(billboardedWorldPos.xyz, 1.0f), Mat_View).xyz;*/
    
    
    float4 normal = float4(_in.normal, 0.0f);
    normal = mul(normal, transpose(Mat_World));
    normal = mul(cameraRotMatrix, normal);
    normal = mul(normal, transpose(Mat_World_Inv));
    
    float4 tangent = float4(_in.tangent, 0.0f);
    tangent = mul(tangent, transpose(Mat_World));
    tangent = mul(cameraRotMatrix, tangent);
    tangent = mul(tangent, transpose(Mat_World_Inv));
    
    float4 binormal = float4(_in.binormal, 0.0f);
    binormal = mul(binormal, transpose(Mat_World));
    binormal = mul(cameraRotMatrix, binormal);
    binormal = mul(binormal, transpose(Mat_World_Inv));
    
    output.view_normal = normalize(mul(float4(normal.xyz, 0.f), Mat_WV)).xyz;
    output.view_tangent     = normalize(mul(float4(tangent.xyz, 0.f), Mat_WV)).xyz;
    output.view_binormal    = normalize(mul(float4(binormal.xyz, 0.f), Mat_WV)).xyz;
    
    
    return output;
}