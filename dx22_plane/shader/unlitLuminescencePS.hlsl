#include "common.hlsl"

// ピクセルシェーダーのエントリポイント
float4 ps_main(in PS_IN input) : SV_Target
{
    float4 color;

    float3 BaseColor = input.col.rgb; // 通常色
    
    float2 uv = input.tex;
    float2 d = uv - 0.5f;
    
    float s, c;
    sincos(Angle, s, c); // 回転を使う
    float2x2 rotationMtx = float2x2(c, -s, s, c);
    d = mul(rotationMtx, d);
        
    float2 scale = EllipseScale;
    d *= scale;
    
    float dist = sqrt(dot(d, d));
    
    // グロー量（外に行くほど減衰）
    float glow = saturate(1.0f - dist / GlowRadius);
    glow = glow * glow; // 二乗して中心部を強調
    
    float3 colorRGB = BaseColor + GlowColor.rgb * glow * GlowPower;
    
    float alpha = glow; // グロー量をアルファに利用
    
    return float4(colorRGB, alpha * input.col.a);
}
