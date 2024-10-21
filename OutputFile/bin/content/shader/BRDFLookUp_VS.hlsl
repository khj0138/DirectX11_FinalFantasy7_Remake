struct VSIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0; 
};

struct VSOut
{
    float4 pos : SV_POSITION; 
    float2 uv : TEXCOORD0; 
};

VSOut main(VSIn input)
{
    VSOut output;
    output.pos = float4(input.pos.xy * 2.0f, 0.0, 1.0);
    output.uv = input.uv; 

    return output;
}