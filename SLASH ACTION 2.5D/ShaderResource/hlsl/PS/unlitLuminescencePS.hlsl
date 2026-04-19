#include "../common.hlsl"

// ピクセルシェーダーのエントリポイント
float4 ps_main(in PS_IN input) : SV_Target
{
    float3 BaseColor = input.col.rgb; // 通常色
    
    // uvの中心を基準に距離を計算
    float2 uv = input.tex * 2.0f -1.0f; // これで描画可能範囲を広げる
    float2 d = uv;
    
    float s, c;
    sincos(Angle, s, c); // 回転を使うので行列生成
    float2x2 rotationMtx = float2x2(c, -s, s, c);
    d = mul(rotationMtx, d);
    
    // 楕円形にも対応できるようスケール設定
    d *= EllipseScale;
    
    // 中心から離れれば離れるほどボヤける
    float dist = sqrt(dot(d, d));
    
    // グロー量（外に行くほど減衰）
    float glow = saturate(1.0f - dist / GlowRadius);
    glow *= glow; // 二乗して中心部を強調
    
    float3 colorRGB = BaseColor + GlowColor.rgb * glow * GlowPower;
    
    float alpha = glow; // グロー量をアルファに利用
    
    return float4(colorRGB, alpha * input.col.a);
}
