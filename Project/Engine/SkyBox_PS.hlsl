#include "Value.hlsli"
#include "Func.hlsli"

#define Skybox_Type Int_0

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    
    float3 dir : POSITION;
};  


float4 main(VSOut _in) : SV_TARGET
{
    float4 out_color = (float4) 0.0f;
    
    if (Skybox_Type == 0)
    {
        if (BTex_0)
        {
            out_color = Tex_0.Sample(Sampler_Anisotropic, _in.uv);
        }
    }
    else if (Skybox_Type == 1)
    {
        if (BCube_0)
        {
            out_color = Cube_0.Sample(Sampler_Anisotropic, normalize(_in.dir));
        }
    }
   
    
    out_color.rgb = ToneMapping(out_color.rgb * 0.02f);
    
    
    return out_color;
}