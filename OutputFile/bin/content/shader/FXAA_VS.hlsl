//struct VSIn
//{
//    float3 pos : POSITION;
//    float2 uv : Texcoord;
//};

//struct VSOut
//{
//    float4 pos : SV_Position;
//    float2 uv : Texcoord;
//};

//VSOut main(VSIn _in)
//{
//    VSOut output;
//    output.uv = _in.uv;
//    output.pos = float4(_in.pos.xy * 2.f, 1.0f, 1.0f);
    
//    return output;
//}


//#define FXAA_PRESET 3
//#define FXAA_HLSL_4 1
////#include ¡°FxaaShader.h¡±
//cbuffer cbFxaa : register(b1)
//{
//    float4 rcpFrame : packoffset(c0);
//};
struct FxaaVS_Output
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};
FxaaVS_Output main(uint id : SV_VertexID)
{
    FxaaVS_Output Output;
    Output.Tex = float2((id << 1) & 2, id & 2);
    Output.Pos = float4(Output.Tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    
    return Output;
}