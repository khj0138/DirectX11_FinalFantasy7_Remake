
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
    
    output.pos = float4(_in.pos * 2.0f, 1.0f);
    
    return output;
}