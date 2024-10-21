
struct VSIn
{
    float3 pos : POSITION;
    uint id : SV_InstanceID;
};

struct VSOut
{
    float3 pos : POSITION;
    uint id : SV_InstanceID;
};

VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.0f;
    
    output.pos = _in.pos;
    output.id = _in.id;
    
    return output;
}