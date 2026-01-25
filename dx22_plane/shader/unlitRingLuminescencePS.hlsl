#include "common.hlsl"

// ピクセルシェーダーのエントリポイント
float4 ps_main(in PS_IN input) : SV_Target
{
    float3 BaseColor = input.col.rgb; // 通常色
    
    // uvの中心を基準に距離を計算
    float2 uv = input.tex * 2.0f - 1.0f; // これで描画可能範囲を広げる
    float2 d = uv;
    
    float s, c;
    sincos(Angle2, s, c); // 回転を使うので行列生成
    float2x2 rotationMtx = float2x2(c, -s, s, c);
    d = mul(rotationMtx, d);
    
    // 楕円形にも対応できるようスケール設定
    d *= EllipseScale2;
    
    float dist = length(d);

    float value = (dist - RingRadius) / RingWidth;
    
// ガウス分布っぽいリング
    float ring = exp(-(value * value));
        
    float3 colorRGB = BaseColor + GlowColor2.rgb * ring * GlowPower2;
    float alpha = ring * GlowPower2;

    
    return float4(colorRGB, alpha * input.col.a);
}
