
struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOut main(uint id : SV_VertexID)
{
    VSOut output;
    
    output.uv = float2((id << 1) & 2, id & 2);
    output.pos = float4(output.uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    
    return output;
}