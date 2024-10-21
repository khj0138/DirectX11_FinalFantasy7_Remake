
struct VSIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.0f;
    
    output.pos = float4(_in.pos * 2.0f, 1.0f);
    output.uv = _in.uv;
    
    return output;
}