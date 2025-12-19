#include "common.hlsl"

PS_IN vs_main(in VS_IN input)
{
    PS_IN output = (PS_IN) 0;

	//positoin=============================
	// ワールド、ビュー、プロジェクション行列を掛け合わせて座標変換を行う
    matrix wvp;
    wvp = mul(matrixWorld, View2D);
    wvp = mul(wvp, Projection2D);
	
    float4 newPos = float4(input.pos, 1.0f); // 入力頂点座標をfloat4に変換(w = 1)
	
    output.pos = mul(newPos, wvp);

	/// color=======================================
	// 法線ベクトルの変換と正規化を行う
	// 入力された法線をワールド空間に変換し、正規化する
    float4 normal = float4(input.nrm.xyz, 0.0); // 入力法線をfloat4に変換(w = 0)
    float4 worldNormal = mul(normal, matrixWorld); // 法線をワールド空間に変換
	// worldNormal = normalize(worldNormal * 0.5f);		// 法線ベクトルを半分に
	//float4 randomNormal = float4(input.nrm.xyz + float3(0.5, 0.5, 0.5), 0.0);
    worldNormal = normalize(worldNormal); // 法線ベクトルを正規化
//	worldNormal = normalize(mul(randomNormal,World));		// 法線ベクトルを正規化

	// 光源方向と法線ベクトルの内積を計算して拡散光を求める
    float d = -dot(Light.Direction.xyz, worldNormal.xyz); // 光の方向と内積を計算
	
    d = saturate(d); // dの値を０～１にクランプ

	// output.col.xyz = input.col.xyz *d * Light.Diffuse.xyz * float3(1.0f, 0.0f, 0.0f); // 赤い光になる
//	output.col.xyz = lerp(input.col.xyz,float3(0.0,1.0,0.0), d); // 緑色へのグラデーション？
    output.col.xyz = input.col.xyz * d * Light.Diffuse.xyz; // 拡散光の影響を乗算
	// output.col.xyz += input.col.xyz * (Light.Ambient.xyz * 0.5f); // アンビエント光を半減(強度変更)
    output.col.xyz += input.col.xyz * Light.Ambient.xyz; // アンビエント光を加算
	//output.col.xyz += (Material.Emission.xyz * 2.0f);	// Emissionを倍増
	
    output.col.xyz += Material.Emission.xyz; // Emissionを加算
    output.col.a = input.col.a * Material.Diffuse.a; // アルファ値はそのまま使用

	//texture=============================
	// テクスチャ座標はそのまま使用
    output.tex = input.tex;
   // output.col.a = vertexColor.a;
	// output.tex = input.tex * 2.0;	// テクスチャ２倍？
	
    output.col *= vertexColor; // 定数バッファの頂点色を乗算
		
    return output;
}
