
#include "Value.hlsli"
#include "BRDF.hlsli"


// Fragment Shader
//uniform samplerCube environmentMap; // È¯°æ¸Ê
//uniform float sampleDelta; // »ùÇÃ¸µ °£°Ý
#define EnvironmentMap  Cube_0  

float sampleDelta = 0.2f;
float3 ApproximateIrradiance(float3 direction);
float2 SphericalSample(float3 v);

//struct VSOut
//{
//    float4 pos : SV_POSITION;
//};

struct VSOut
{
    float4 pos : SV_Position;
    //float3 dir : POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VSOut _in) : SV_TARGET
{
    //return float4(1.0f, 0.0f, 1.0f, 1.0f); // float3(_pos.x, 1.0f, -_pos.y);
    float3 dir = SphericalToDirection(_in.uv); //normalize(float3(_in.dir.x, 1.0f, -_in.dir.y)); // ÇöÀç ÇÈ¼¿ÀÇ ¹æÇâ
    //return float4(dir.x * 0.5f + 0.5f, dir.y * 0.5f + 0.5f, dir.z * 0.5f + 0.5f, 1.0f);
    float3 indirectLight = ApproximateIrradiance(dir);
    //return float4(EnvironmentMap.Sample(Sampler_0, dir).rgb, 1.0f);
    return float4(indirectLight, 1.0);
}

float2 SphericalSample(float3 v)
{
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb509575(v=vs.85).aspx
	// The signs of the x and y parameters are used to determine the quadrant of the return values 
	// within the range of -PI to PI. The atan2 HLSL intrinsic function is well-defined for every point 
	// other than the origin, even if y equals 0 and x does not equal 0.
    float2 uv = float2(atan2(v.z, v.x), asin(-v.y));
    uv /= float2(-PI * 2.0f, PI);
    uv += float2(0.5, 0.5);
    return uv;
}


float3 ApproximateIrradiance(float3 direction)
{
    //return EnvironmentMap.Sample(Sampler_1, direction).rgb;
    
    float3 irradiance = (float3) 0.0;
    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = cross(up, direction);
    up = cross(direction, right);
 
    float sampleStep = 0.025f;
    int numSamples = 0;

    for (float phi = 0.0; phi < 2.0f * PI; phi += sampleStep)
    {
        for (float theta = 0.0; theta < 0.5f * PI; theta += sampleStep)
        {
            float3 sampleDirection = direction * cos(theta) + right * sin(theta) * cos(phi) + up * sin(theta) * sin(phi);
            irradiance += EnvironmentMap.Sample(Sampler_Linear, sampleDirection).rgb; //texture(EnvironmentMap, sampleDirection).rgb;
            numSamples++;
        }
    }

    irradiance = irradiance / float(numSamples);
    return irradiance;
}