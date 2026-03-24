#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

// ピクセルシェーダーのエントリポイント
float4 ps_main(in PS_IN input) : SV_Target
{
    float4 texColor = g_Texture.Sample(g_SamplerState, input.tex);
    
    float3 BaseColor = input.col.rgb; // 通常色
    
    BaseColor *= texColor.rgb; // テクスチャの色を乗算して基本色を決定
    
    // uvの中心を基準に距離を計算
    float2 uv = input.tex * 2.0f - 1.0f; // これで描画可能範囲を広げる
    
  //  float4 GlowColor = float4(1.0f, 1.0f, 1.0f, 1.0f); // グローの色
    
    //float s, c;
    //sincos(Angle, s, c); // 回転を使うので行列生成
    //float2x2 rotationMtx = float2x2(c, -s, s, c);
    //d = mul(rotationMtx, d);
    
    // 楕円形にも対応できるようスケール設定
 //   d *= EllipseScale;
    
    // 中心から離れれば離れるほどボヤける
    float dist = sqrt(dot(uv, uv));
    
    // グロー量（外に行くほど減衰）
    //float glow = saturate(1.0f - dist / GlowRadius);
    //glow *= glow; // 二乗して中心部を強調
    
    float glow = texColor.a;
    
    float3 colorRGB = BaseColor + GlowColor.rgb * glow * GlowPower;
    
    //float alpha = glow; // グロー量をアルファに利用
    float alpha = texColor.a * input.col.a; // グロー量をアルファに利用
    
   // alpha *= texColor.a; // テクスチャのアルファも考慮
    
    return float4(colorRGB, alpha);
}
