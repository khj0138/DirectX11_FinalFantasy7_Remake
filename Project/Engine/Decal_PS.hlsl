#include "Value.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
        
    float3 view_pos : POSITION;
    
    float3 view_normal : NORMAL;
    float3 view_tangent : TANGENT;
    float3 view_binormal : BINORMAL;
};

#define PositionTex         Tex_0
#define OutputTex           Tex_1
#define IsOutputTex         BTex_1

#define Emissive_Factor         Float_0

#define UV_X_Boundary           Vec2_0
#define UV_Y_Boundary           Vec2_1
#define UV_Y_Discard_Boundary   Vec2_2
#define Color_Parametr          Vec4_0

float4 main(VSOut _in) : SV_TARGET
{
    float4 out_color = (float4) 0.f;
    
    // 데칼오브젝트 영역에 해당하는 Position 값을 가져온다.
    float2 vScreenUV = _in.pos.xy / Resolution;
    float3 vViewPos = PositionTex.Sample(Sampler_Anisotropic, vScreenUV).xyz;
    float3 vWorldPos = mul(float4(vViewPos, 1.f), Mat_View_Inv).xyz;
    float3 vLocal = mul(float4(vWorldPos, 1.f), Mat_World_Inv).xyz;
    
    if (abs(vLocal.x) < 0.5f && abs(vLocal.y) < 0.5f && abs(vLocal.z) < 0.5f)
    {       
        float2 uv = vLocal.xz;
        uv.x = uv.x + 0.5f;
        uv.y = (1.f - uv.y) - 0.5f;
        
        if (IsOutputTex)
        {
            out_color = OutputTex.Sample(Sampler_Anisotropic, uv);
            out_color.a = out_color.r;

        }
        else
        {
            discard;
        }
        
        if (out_color.a <= 0.03f)
        {
            discard;
        }

        if (uv.y < UV_Y_Boundary.x || uv.y > UV_Y_Boundary.y)
        {
            discard;
        }
        else if (uv.x < UV_X_Boundary.x || uv.x > UV_X_Boundary.y)
        {
            discard;
        }
    
        if (uv.y < UV_Y_Discard_Boundary.y && uv.y > UV_Y_Discard_Boundary.x)
        {
            discard;
        }
    

        out_color *= Color_Parametr;
        out_color.rgb = min(out_color.rgb, float3(1.0f, 1.0f, 1.0f));

        if (Emissive_Factor > 0.0f)
        {
            out_color.rgb *= Emissive_Factor;
        }
    }
    else
    {     
        discard;
    }
   
    
    return out_color;

}