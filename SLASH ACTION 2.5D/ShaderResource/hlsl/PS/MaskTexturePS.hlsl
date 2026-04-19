#include "../common.hlsl"

Texture2D tex1 : register(t0);		// 通常テクスチャ
Texture2D mask : register(t1);	// マスクテクスチャ
SamplerState samp : register(s0); // サンプラー

float4 ps_main(in PS_IN input) : SV_TARGET
{
    float4 color = tex1.Sample(samp, input.tex);
    float maskValue = mask.Sample(samp, input.tex).r; // 赤チャンネル（0～1）

    // マスクに応じてアルファを調整
    color.a *= maskValue;

    return color;
}