#include "Value.hlsli"
#include "BRDF.hlsli"

struct VSOut
{
    float4 pos : SV_POSITION;
    float3 dir : TEXCOORD0;
};

#define HDR_Image Tex_0

//struct PS_OUT
//{
//    float4 right    : SV_Target0;
//    float4 left     : SV_Target1;
//    float4 up       : SV_Target2;
//    float4 down     : SV_Target3;
//    float4 front    : SV_Target4;
//    float4 behind   : SV_Target5;
//};

//float2 EquirectangularToUV(float3 pos);

float4 main(VSOut _in) : SV_TARGET
{
    float4 output = (float4) 0.0f;
    float2 uv = (EquirectangularToUV(normalize(_in.dir))); //_in.uv; //EquirectangularToUV(normalize(_in.dir)); // 구형 매핑에서 UV 매핑으로 변환
    
    //if (dir.y >= 0.f && dir.y > (sqrt(2.0f) / 2.0f))
    //{
    //    output.up = HDR_Image.Sample(Sampler_0, _in.uv);

    //}
    //if (uv.x < 0 || uv.x > 1 || uv.y< 0 || uv.y > 1)
    //    return float4(1.0f, 0.0f, 1.0f, 1.0f);
    output = HDR_Image.Sample(Sampler_Anisotropic, uv);
    return output;
    // HDR 텍스처 샘플링
    
    //float2 leftUV = uv;
    //leftUV.x /= 4.0f;
    //leftUV.y /= 3.0f;
    //leftUV.y += 1.0f / 3.0f;
    //output.left = HDR_Image.Sample(Sampler_0, leftUV);
    //
    //float2 rightUV = uv;
    //rightUV.x /= 4.0f;
    //rightUV.x += 2.0f / 4.0f;
    //rightUV.y /= 3.0f;
    //rightUV.y += 1.0f / 3.0f;
    //output.right = HDR_Image.Sample(Sampler_0, rightUV);
    //
    //float2 frontUV = uv;
    //frontUV.x /= 4.0f;
    //frontUV.x += 1.0f / 4.0f;
    //frontUV.y /= 3.0f;
    //frontUV.y += 1.0f / 3.0f;
    //output.front = HDR_Image.Sample(Sampler_0, frontUV);
    //
    //float2 behindUV = uv;
    //behindUV.x /= 4.0f;
    //behindUV.x += 3.0f / 4.0f;
    //behindUV.y /= 3.0f;
    //behindUV.y += 1.0f / 3.0f;
    //output.behind = HDR_Image.Sample(Sampler_0, behindUV);
    //
    //float2 upUV = uv;
    //upUV.x /= 4.0f;
    //upUV.x += 1.0f / 4.0f;
    //upUV.y /= 3.0f;
    //output.up = HDR_Image.Sample(Sampler_0, upUV);
    //
    //float2 downUV = uv;
    //downUV.x /= 4.0f;
    //downUV.x += 1.0f / 4.0f;
    //downUV.y /= 3.0f;
    //downUV.y += 2.0f / 3.0f;
    //output.down = HDR_Image.Sample(Sampler_0, downUV);
    //
    //return output;
}

// 구형 매핑에서 UV 매핑으로 변환하는 함수
//float2 EquirectangularToUV(float3 pos)
//{
//    float u = atan2(pos.z, pos.x) / (2.0f * PI) + 0.5f;
//    float v = asin(pos.y) / PI + 0.5f;
//    return float2(u, v);
//}

//float2 SphericalSample(float3 v)
//{
//	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb509575(v=vs.85).aspx
//	// The signs of the x and y parameters are used to determine the quadrant of the return values 
//	// within the range of -PI to PI. The atan2 HLSL intrinsic function is well-defined for every point 
//	// other than the origin, even if y equals 0 and x does not equal 0.
//    float2 uv = float2(atan2(v.z, v.x), asin(-v.y));
//    uv /= float2(-TWO_PI, PI);
//    uv += float2(0.5, 0.5);
//    return uv;
//}



