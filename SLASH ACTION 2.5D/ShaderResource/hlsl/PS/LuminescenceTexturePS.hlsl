#include "../common.hlsl"
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float4 ps_main(in PS_IN input) : SV_Target
{
    // テクスチャカラーは不要、アルファの形だけ使う
    float alpha = g_Texture.Sample(g_SamplerState, input.tex).a;

    // 広範囲サンプリングでアルファをぼかす
    float glowAlpha = 0.0;
    const int STEPS = 8;
    const float MAX_OFFSET = 0.05;

    for (int i = 1; i <= STEPS; i++)
    {
        float r = (float) i / (float) STEPS * MAX_OFFSET;
        float weight = 1.0 - (float) i / (float) STEPS;

        float s = 0.0;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(r, 0)).a;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(-r, 0)).a;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(0, r)).a;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(0, -r)).a;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(r, r) * 0.707).a;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(-r, r) * 0.707).a;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(r, -r) * 0.707).a;
        s += g_Texture.Sample(g_SamplerState, input.tex + float2(-r, -r) * 0.707).a;
        s /= 8.0;

        glowAlpha += s * weight;
    }
    glowAlpha = saturate(glowAlpha / (STEPS * 0.5));
    glowAlpha *= GlowColor.a;
    
    // グロー色と強度だけ出力、テクスチャ色は一切無視
    float GlowPower = 2.0;    
        
    return float4(GlowColor.rgb * glowAlpha * GlowPower, glowAlpha);
}