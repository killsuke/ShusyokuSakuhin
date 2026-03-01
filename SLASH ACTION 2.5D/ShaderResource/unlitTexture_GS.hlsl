#include "common.hlsl"

[maxvertexcount(4)]
void gs_main(line PS_IN input[2], inout TriangleStream<PS_IN> output)
{
    // 線の端を取得
    float4 start = input[0].pos;
    float4 end = input[1].pos;

    // ワールド座標をビュー空間に変換
    float4 startVS = mul(start, View3D);
    float4 endVS = mul(end, View3D);

    // ビュー空間での方向ベクトル
    float3 dir = normalize(endVS.xyz - startVS.xyz);

    // カメラ正面に垂直な方向を求める
    // ここでcameraForwardを(0,0,1)にしているのは、
    // カメラが実質世界の中心となるため、カメラから見た世界が全て正しい
    // なので、(0,0,1) == カメラの前方向となる
    float3 cameraForward = float3(0, 0, 1); // ビュー空間ではZ+が前向き
    float3 right = normalize(cross(dir, cameraForward));
       
    right *= lineThickness * 0.5f;

    // ４頂点生成
    PS_IN v = (PS_IN)0;
    
    // Appendで頂点を追加 × 4
    // →３角形を２枚作って長方形を描く

    // 左下
    v.pos = mul(startVS + float4(-right, 0), Projection3D);
    v.col = input[0].col;
    v.tex = float2(1, 0); // 左下
    output.Append(v);

    // 左上
    v.pos = mul(endVS + float4(-right, 0), Projection3D);
    v.col = input[1].col;
    v.tex = float2(1, 1); // 左上
    output.Append(v);

    // 右下
    v.pos = mul(startVS + float4(right, 0), Projection3D);
    v.col = input[0].col;
    v.tex = float2(0, 0); // 右下
    output.Append(v);

    // 右上
    v.pos = mul(endVS + float4(right, 0), Projection3D);
    v.col = input[1].col;
    v.tex = float2(0, 1); // 右上
    output.Append(v);

    output.RestartStrip(); // ２枚目の三角形を描くためにストリップをリスタート（四角形であればほぼ必要ないが念のため）
}