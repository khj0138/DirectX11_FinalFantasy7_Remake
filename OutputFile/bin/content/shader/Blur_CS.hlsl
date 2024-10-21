#include "Value.hlsli"

#define Width Float_0
#define Height Float_1

#define Index Int_0

#define Src_Texture Tex_1

RWTexture2D<float4> Target_Texture : register(u0);

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    float4 color = (float4) 0.0f;
    
    if (DTid.x >= Width || DTid.y >= Height)
    {
        return;
    }
    
    if (Width <= 200.f)
    {
        if (Index % 2 == 0)
        {
            for (int i = -1; i <= 1; i++)
            {
                if (DTid.x + i < 0 || DTid.x + i >= Width)
                {
                    continue;
                }
        
                color += Blur_Weight0[i + 1] * Src_Texture[uint2(DTid.x + i, DTid.y)];
            }
        }
        else
        {
            for (int i = -1; i <= 1; i++)
            {
                if (DTid.y + i < 0 || DTid.y + i >= Height)
                {
                    continue;
                }
        
                color += Blur_Weight0[i + 1] * Src_Texture[uint2(DTid.x, DTid.y + i)];
            }
        }
    }
    else if (Width <= 400.f)
    {
        if (Index % 2 == 0)
        {
            for (int i = -2; i <= 2; i++)
            {
                if (DTid.x + i < 0 || DTid.x + i >= Width)
                {
                    continue;
                }
        
                color += Blur_Weight1[i + 2] * Src_Texture[uint2(DTid.x + i, DTid.y)];
            }
        }
        else
        {
            for (int i = -2; i <= 2; i++)
            {
                if (DTid.y + i < 0 || DTid.y + i >= Height)
                {
                    continue;
                }
        
                color += Blur_Weight1[i + 2] * Src_Texture[uint2(DTid.x, DTid.y + i)];
            }
        }
    }
    else if (Width <= 800.f)
    {
        if (Index % 2 == 0)
        {
            for (int i = -3; i <= 3; i++)
            {
                if (DTid.x + i < 0 || DTid.x + i >= Width)
                {
                    continue;
                }
        
                color += Blur_Weight2[i + 3] * Src_Texture[uint2(DTid.x + i, DTid.y)];
            }
        }
        else
        {
            for (int i = -3; i <= 3; i++)
            {
                if (DTid.y + i < 0 || DTid.y + i >= Height)
                {
                    continue;
                }
        
                color += Blur_Weight2[i + 3] * Src_Texture[uint2(DTid.x, DTid.y + i)];
            }
        }
    }
    
    Target_Texture[DTid.xy] = float4(color.rgb,0.5f);
}