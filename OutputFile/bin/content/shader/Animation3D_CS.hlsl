#include "Value.hlsli"
#include "Anim.hlsli"

struct tFrameTrans
{
    float4 trans;
    float4 scale;
    float4 quter_rot;
};

StructuredBuffer<tFrameTrans> Frame_Trans : register(t16);
StructuredBuffer<matrix> g_arrOffset : register(t17);
RWStructuredBuffer<matrix> g_arrFinelMat : register(u0);
RWStructuredBuffer<tFrameTrans> Last_Frame_Trans : register(u1);
RWStructuredBuffer<matrix> Joint_Mat : register(u2);

// ===========================
// Animation3D Compute Shader
#define Joint_Cnt               Int_0
#define Cur_Frame               Int_1
#define Next_Frame              Int_2
#define Change_Anim             Bool_0
#define Start_Change_Anim       Bool_1
#define Ratio                   Float_0
// ===========================

[numthreads(256, 1, 1)]
void main(int3 DTid : SV_DispatchThreadID)
{
    if (Joint_Cnt <= DTid.x)
    {
        return;
    }

    // 오프셋 행렬을 곱하여 최종 본행렬을 만들어낸다.		
    float4 vQZero = float4(0.f, 0.f, 0.f, 1.f);
    matrix matBone = (matrix) 0.f;

    uint cur_frame_idx = Joint_Cnt * Cur_Frame + DTid.x;
    uint next_fram_idx = Joint_Cnt * Next_Frame + DTid.x;

    float4 scale;
    float4 trans;
    float4 quter_rot;

    if (Change_Anim)
    {
        //if (Ratio <= 0.0f)
        //{
        //    scale = Frame_Trans[cur_frame_idx].scale;

        //    trans = Frame_Trans[cur_frame_idx].trans;

        //    quter_rot = Frame_Trans[cur_frame_idx].quter_rot;
        //}
        //else
        {
            scale = lerp(Last_Frame_Trans[DTid.x].scale, Frame_Trans[cur_frame_idx].scale, Ratio);
            trans = lerp(Last_Frame_Trans[DTid.x].trans, Frame_Trans[cur_frame_idx].trans, Ratio);
            quter_rot = QuternionLerp(Last_Frame_Trans[DTid.x].quter_rot, Frame_Trans[cur_frame_idx].quter_rot, Ratio);
        }
       // scale = lerp(Last_Frame_Trans[DTid.x].scale, Frame_Trans[cur_frame_idx].scale, Ratio);
       // trans = lerp(Last_Frame_Trans[DTid.x].trans, Frame_Trans[cur_frame_idx].trans, Ratio);
       // quter_rot = QuternionLerp(Last_Frame_Trans[DTid.x].quter_rot, Frame_Trans[cur_frame_idx].quter_rot, Ratio);
        

    }
    else
    {
        scale = lerp(Frame_Trans[cur_frame_idx].scale, Frame_Trans[next_fram_idx].scale, Ratio);
        trans = lerp(Frame_Trans[cur_frame_idx].trans, Frame_Trans[next_fram_idx].trans, Ratio);
        quter_rot = QuternionLerp(Frame_Trans[cur_frame_idx].quter_rot, Frame_Trans[next_fram_idx].quter_rot, Ratio);
    }
    
    if (Start_Change_Anim)
    {
        Last_Frame_Trans[DTid.x].scale = scale;
        Last_Frame_Trans[DTid.x].trans = trans;
        Last_Frame_Trans[DTid.x].quter_rot = quter_rot;
    }
    

    // 최종 본행렬 연산    
    MatrixAffineTransformation(scale, vQZero, quter_rot, trans, matBone);
    // 최종 본행렬 연산    
    //MatrixAffineTransformation(g_arrFrameTrans[iFrameDataIndex].vScale, vQZero, g_arrFrameTrans[iFrameDataIndex].qRot, g_arrFrameTrans[iFrameDataIndex].vTranslate, matBone);

    matrix matOffset = transpose(g_arrOffset[DTid.x]);
    
    // 구조화버퍼에 결과값 저장
    g_arrFinelMat[DTid.x] = mul(matOffset, matBone);    
    
    Joint_Mat[DTid.x] = transpose(matBone);
}