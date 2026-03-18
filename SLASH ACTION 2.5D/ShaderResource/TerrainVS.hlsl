#include "common.hlsl"

PS_SHADOW vs_main(in VS_IN input)
{
    PS_SHADOW output = (PS_SHADOW) 0;

	//positoin=============================
	// ワールド、ビュー、プロジェクション行列を掛け合わせて座標変換を行う
    matrix wvp;
    wvp = mul(matrixWorld, View3D);
    wvp = mul(wvp, Projection3D);
	
    float4 newPos = float4(input.pos, 1.0f); // 入力頂点座標をfloat4に変換(w = 1)
	
    output.pos = mul(newPos, wvp);

    output.height = input.pos.y; // 頂点の高さを出力に格納
    output.worldPos = mul(newPos, matrixWorld); // ワールド空間の座標を計算して出力に格納
    
	/// color=======================================
	// 法線ベクトルの変換と正規化を行う
	// 入力された法線をワールド空間に変換し、正規化する
    float4 normal = float4(input.nrm.xyz, 0.0); // 入力法線をfloat4に変換(w = 0)
    float4 worldNormal = mul(normal, matrixWorld); // 法線をワールド空間に変換
    worldNormal = normalize(worldNormal); // 法線ベクトルを正規化

    output.worldNormal = worldNormal;
    
	// 光源方向と法線ベクトルの内積を計算して拡散光を求める
    float d = -dot(Light.Direction.xyz, worldNormal.xyz); // 光の方向と内積を計算
	
    d = saturate(d); // dの値を０～１にクランプ

    output.col.xyz = input.col.xyz * d * Light.Diffuse.xyz; // 拡散光の影響を乗算
    output.col.xyz += input.col.xyz * Light.Ambient.xyz; // アンビエント光を加算
	
    output.col.xyz += Material.Emission.xyz; // Emissionを加算
    output.col.a = input.col.a * Material.Diffuse.a; // アルファ値はそのまま使用

	//texture=============================
	// テクスチャ座標はそのまま使用
    output.tex = input.tex;
	
    output.col *= vertexColor; // 定数バッファの頂点色を乗算
    
    return output;
}
