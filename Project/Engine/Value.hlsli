
#ifndef _VALUE
#define _VALUE

#include "Struct.hlsli"

static const float PI = 3.14159265f;
static const float EPSILON = 1e-6f;

cbuffer TRANSFORM : register(b0)
{
    row_major matrix Mat_World;
    row_major matrix Mat_World_Inv;
    row_major matrix Mat_View;
    row_major matrix Mat_View_Inv;
    row_major matrix Mat_Proj;
    row_major matrix Mat_Proj_Inv;
    
    row_major matrix Mat_WV;
    row_major matrix Mat_WVP;
};

cbuffer MATERIAL : register(b1)
{
    float4  Color_Factor;
    float3  Emissive_Factor;
    float   Metalic_Factor;
    float   Roughness_Factor;
 
    float3  Factor_Padding;
    
    int Int_0;
    int Int_1;
    int Int_2;
    int Int_3;
    
    bool Bool_0;
    bool Bool_1;
    bool Bool_2;
    bool Bool_3;
    
    //int3 temp;

    
    float Float_0;
    float Float_1;
    float Float_2;
    float Float_3;
    
    float2 Vec2_0;
    float2 Vec2_1;
    float2 Vec2_2;
    float2 Vec2_3;  

    float4 Vec4_0;
    float4 Vec4_1;
    float4 Vec4_2;
    float4 Vec4_3;
    
    row_major matrix Mat_0;
    row_major matrix Mat_1;
    row_major matrix Mat_2;
    row_major matrix Mat_3;
    
    int BTex_0;
    int BTex_1;
    int BTex_2;
    int BTex_3;
    int BTex_4;
    int BTex_5;
    int BTex_6;
    int BTex_7;
    
    int BCube_0;
    int BCube_1;
    
    int BArr_Tex_0;
    int BArr_Tex_1;
    
    int g_iAnim;
    int g_iBoneCount;
    
    int Socket_Index;
    int padding;
};

cbuffer GLOBAL : register(b2)
{
    float2  Resolution;
    float   DT;
    float   Acc_Time;
    
    float3  Camera_Pos;
    
    float3  Ambient_Color;
    float   Ambient_Intensity;
    
    int    Light_Cnt;
}

cbuffer Capture : register(b3)
{
    uint object_cnt;
    uint min_level;
    uint max_level;
}


Texture2D Tex_0 : register(t0);
Texture2D Tex_1 : register(t1);
Texture2D Tex_2 : register(t2);
Texture2D Tex_3 : register(t3);
Texture2D Tex_4 : register(t4);
Texture2D Tex_5 : register(t5);
Texture2D Tex_6 : register(t6);
Texture2D Tex_7 : register(t7);

TextureCube Cube_0 : register(t8);
TextureCube Cube_1 : register(t9);

Texture2DArray Arr_Tex_0 : register(t10);
Texture2DArray Arr_Tex_1 : register(t11);

StructuredBuffer<LightInfo> Lights : register(t12);
StructuredBuffer<CaptureInfo> Captures : register(t13);

TextureCubeArray Cube_Array : register(t14);
Texture2DArray Tex_Array : register(t15);


StructuredBuffer<Matrix> JointMat : register(t30);
StructuredBuffer<Matrix> JointMatT : register(t31);

SamplerState Sampler_Anisotropic : register(s0);
SamplerState Sampler_Point  : register(s1);
SamplerState Sampler_Linear : register(s2);
SamplerState Sampler_Min_Mag_Linear : register(s3); // Max LOD : 5.0f
SamplerState Sampler_LOD_Linear : register(s4);


//const static float Blur_Weight[13] = 
//{
//    0.004f, 0.017f, 0.040f, 0.082f, 0.117f, 0.148f, 0.168f, 0.148f, 0.117f, 0.082f, 0.040f, 0.017f, 0.004f
//};

//const static float Blur_Weight0[3] =
//{
//    0.31527, 0.369459, 0.31527
//};

const static float Blur_Weight0[3] =
{
    0.1, 0.8, 0.1
};

//const static float Blur_Weight1[5] =
//{
//    0.140748, 0.226523, 0.265458, 0.226523, 0.140748
//};

const static float Blur_Weight1[5] =
{
    0.1, 0.2, 0.4, 0.2, 0.1
};

//const static float Blur_Weight2[7] =
//{
//    0.056477, 0.12485, 0.200936, 0.235473, 0.200936, 0.12485, 0.056477
//};

const static float Blur_Weight2[7] =
{
    0.05, 0.1, 0.15, 0.4, 0.15, 0.1, 0.05

};
#endif