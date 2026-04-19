#include "../common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D shadowTex : register(t1);
SamplerState g_SamplerState : register(s0);
SamplerState shadowSampler : register(s1);

float4 ps_main(in PS_SHADOW input) : SV_Target
{
    float4 color = float4(1, 1, 1, 1); // 初期化

    float shadow = 0.0f;

    for (int i = 0; i < SHADOW_OBJECT_MAX; i++)
    {
        if (i >= shadowCount)
            break;
        
        float height = input.worldPos.y - shadows[i].objectPos.y; // オブジェクトとピクセルの高さ差を計算
        
        // オブジェクトより下にある場合のみ影を計算
        if (height > 0.0f)
            continue;

        float heightFade = saturate(1.0f - height * 0.1f); // 高さに基づいて影の強さを計算

        float heightAbs = abs(input.worldPos.y - shadows[i].objectPos.y); // オブジェクトとピクセルの高さ差を計算
        
        float heightScale = saturate(1.0f / (heightAbs * 0.04f));
        
        float radius = shadows[i].shadowRadius * heightScale; // 高さに基づいて影の半径を調整
        
        float2 diff = input.worldPos.xz - shadows[i].objectPos.xz; // オブジェクトとピクセルのXZ距離を計算   
        float dist = length(diff); // 距離を計算

        float s = 0.0f;
        
        if (dist < radius) // 影の半径内にある場合のみ影を計算
        {
            s = saturate(1.0f - dist / radius); // 距離に基づいて影の強さを計算
       
            float2 shadowUV = diff / radius; // オブジェクトを中心としたUV座標に変換
            shadowUV = shadowUV * 0.5f + 0.5f; // UV座標を[0,1]範囲に変換
        
            float shadowTexValue = shadowTex.SampleLevel(shadowSampler, shadowUV, 0.0f).r; // 影テクスチャから影の強さをサンプリング
            s *= shadowTexValue; // 影テクスチャの値を影の強さに乗算
        }
        
        float normalMask = saturate(dot(input.worldNormal.xyz, float3(0, 1, 0))); // 上面に影、横面は弱く、下面には出ず
        s *= normalMask; // 法線の向きに基づいて影の強さを調整
            
        s *= heightScale; // 高さによるスケーリングを影の強さに乗算
        
//        s *= heightFade; // 高さによるフェードを影の強さに乗算
        
        shadow = max(shadow, s); // 複数の影がある場合は最大値を取る)
    }
    
    if (Material.TextureEnable)
    {
        // Sample関数→テクスチャから該当のUV位置のピクセル色を取って来る
        //float2 uv = input.tex;
        //uv *= 10.0f; // UVを拡大してタイル状にする
        //color = g_Texture.Sample(g_SamplerState, uv);
        color = g_Texture.Sample(g_SamplerState, input.tex);
        color *= input.col;
    }
    else
    {
        color = input.col * Material.Diffuse;
    }

    color.rgb *= (1.0f - shadow * 1.5f); // 影の強さに応じて色を暗くする
    
    return color;
}