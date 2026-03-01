#include "common.hlsl"

PS_IN vs_main(in VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
    // 現在の位置と過去の位置を計算
    // 現在の位置
    float4 currentPos = float4(input.pos, 1.0f);
    
    currentPos = mul(currentPos, matrixWorld);

    currentPos = mul(currentPos, View3D);

    currentPos = mul(currentPos, Projection3D);

    // ＵＶ座標を移動させる
    float4 uv;
    uv.xy = input.tex; // 行列掛け算のためflaot4型に移す
    uv.z = 0.0f;
    uv.w = 1.0f;
	//uv = mul(uv, matrixTex1);	// ＵＶ座標と移動行列を掛け算
    output.tex = uv.xy; // 掛け算の結果を送信用変数にセット

    output.col = input.col * vertexColor;
    
    if (blurParams.a == 0.0f)
    {
        output.pos = currentPos;
    }
    else if (blurParams.a == 1.0f)
    {
        // 過去の位置
        float4 prevPos = float4(input.pos, 1.0f);
    
        prevPos = mul(prevPos, matrixWorldPrev);

        prevPos = mul(prevPos, View3D);

        prevPos = mul(prevPos, Projection3D);
    
    	// シェルの段階（0.0f～1.0f）
        float t = blurParams.x / blurParams.y;
    
        // blurParams.zを「ブラーの長さ（時間軸のスケール）」として使用
        // 1.0fなら前フレームまで、0.5ならその半分までの軌跡になる
        float lerpFactor = t * blurParams.z;
    
        // 移動方向に沿って頂点をオフセット
        output.pos = lerp(currentPos, prevPos, lerpFactor);
                
        // アルファ値はシェルの段階に応じて変化
        output.col.a = 1.0f - t;
    }
    
    return output;
}