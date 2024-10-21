#include "Value.hlsli"

struct VSIn
{
    float3 pos : POSITION;
};
	
struct VSOut
{
    float4 pos : SV_Position;
};

VSOut main(VSIn _in) 
{
    VSOut output = (VSOut) 0.0f;
    
    output.pos = mul(float4(_in.pos, 1.0f), Mat_WVP);
    
    return output;
}