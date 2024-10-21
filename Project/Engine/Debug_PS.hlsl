#include "Value.hlsli"

	
struct VSOut
{
    float4 pos : SV_Position;
};

#define Color Vec4_0

float4 main(VSOut _in) : SV_Target
{
    float4 output = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    output = Color;
    
    return output;
}   