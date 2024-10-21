#include "Value.hlsli"

struct VSIn
{
    float3 pos : POSITION;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float3 dir : TEXCOORD0;
};

#define Face_Index Int_0

float3 CalCubeDirection(float2 _pos);

//VSOut main(VSIn _in)
//{
//    VSOut output = (VSOut) 0.0f;
//    output.pos = float4(_in.pos.xyz * 2.f, 1.f);
//    output.dir = CalCubeDirection(_in.pos.xy * 2.0f);
    
//    return output;
//}

VSOut main(uint id : SV_VertexID)
{
    VSOut output = (VSOut) 0.0f;
    
    float2 uv = float2((id << 1) & 2, id & 2);
    output.pos = float4(uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    output.dir = CalCubeDirection(output.pos.xy);
    
    return output;
}


float3 CalCubeDirection(float2 _pos)
{
    float3 dir;
    
    switch (Face_Index)
    {
        case 0: // +X
            dir = float3(1.0f, _pos.y, -_pos.x);
            break;
        
        case 1: // -X
            dir = float3(-1.0f, _pos.y, _pos.x);
            break;
        
        case 2: // +Y
            dir = float3(_pos.x, 1.0f, -_pos.y);
            break;

        case 3: // -Y
            dir = float3(_pos.x, -1.0f, _pos.y);
            break;
        
        case 4: // +Z
            dir = float3(_pos.x, _pos.y, 1.0f);
            break;

        case 5: // -Z
            dir = float3(-_pos.x, _pos.y, -1.0f);
            break;
    }
    return dir;
}
