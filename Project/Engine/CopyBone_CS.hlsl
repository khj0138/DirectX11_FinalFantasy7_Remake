#ifndef _COPYBONE
#define _COPYBONE

#include "Value.hlsli"

RWStructuredBuffer<matrix> g_arrDestMat : register(u0); // ������� ������ ����
StructuredBuffer<matrix> g_srcMat : register(t16); // ���� ����

// ======================
// BoneMatrix ���� ���̴�
#define BoneCount   Int_0
#define RowIndex    Int_1
// ======================


[numthreads(1024, 1, 1)]
void main(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= BoneCount)
        return;

    g_arrDestMat[BoneCount * RowIndex + _iThreadIdx.x] = g_srcMat[_iThreadIdx.x];
}

#endif