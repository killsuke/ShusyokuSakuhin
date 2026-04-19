#include "../common.hlsl"

SamplerState samp : register(s0);
Texture2D tex : register(t0);

float4 ps_main(in PS_IN pin) : SV_Target
{    
    float value = 1.0f / 25.0f; // 5x5 のフィルター
    float4 color = float4(0, 0, 0, 0);
    float2 uvOffset = 1.0f / texSize;

    if (Material.TextureEnable)
    {
        for (int y = -2; y <= 2; y++)
        {
            for (int x = -2; x <= 2; x++)
            {
                float2 offset = float2(x, y) * uvOffset;
                color += tex.Sample(samp, pin.tex + offset) * value;
            }
        }
    }
    else
    {
        color = pin.col * Material.Diffuse;
        
        return color;
    }

    color *= pin.col;
    
    return color;
}