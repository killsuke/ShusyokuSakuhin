#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

// ピクセルシェーダーのエントリポイント
float4 ps_main(in PS_IN input) : SV_Target
{
    float4 color;
	    
    // Sample関数→テクスチャから該当のUV位置のピクセル色を取って来る
    color = g_Texture.Sample(g_SamplerState, input.tex);    
    color *= input.col;
    
    // 完全な透明の場合は描かない
    if (color.a < 0.01f)
    {
        discard;
    }
    
    return color;

    // ピクセルシェーダーが動いているかを確かめたいなら
    // return float4(1.0f,0.0f,0.0f,1.0f);
    // を入れて確かめる
}
