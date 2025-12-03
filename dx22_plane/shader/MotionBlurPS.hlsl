#include "common.hlsl"

SamplerState samp : register(s0);
Texture2D tex : register(t0);

float4 ps_main(in PS_IN pin) : SV_Target
{    
    float4 color = float4(0, 0, 0, 0);
    const int sampleCount = 16;
    
    if (Material.TextureEnable)
    {
        float totalWeight = 0.0f;
        float sigma = 0.3f;
        // velocityに沿ってサンプリング
        for (int i = 0; i < sampleCount; i++)
        {
            float t = (i / (float)(sampleCount - 1.0f)); // 0.0 ～ 1.0
            float2 offset = velocity * (t - 0.5); // -0.5 ～ +0.5 の範囲にオフセット
            float weight = exp(-pow((t - 0.5f) / sigma, 2.0f)); // ガウス分布に基づく重み
            color += tex.Sample(samp, pin.tex + offset) * weight;
            totalWeight += weight;
        }
        color /= totalWeight;
    }
    else
    {
        color = pin.col * Material.Diffuse;
        
        return color;
    }

    color *= pin.col;
    
    return color;
}