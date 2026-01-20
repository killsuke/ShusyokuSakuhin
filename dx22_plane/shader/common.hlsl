//cbuffer WorldBuffer : register(b0)
//{
//	matrix World;
//}

struct VS_IN
{
    float3 pos : POSITION0;
    float3 nrm : NORMAL0;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
};

struct VS_ANIMATION
{
    float4 col : COLOR0;
    uint4 idx : BLENDINDICES;

    float3 pos : POSITION;
//    float4 nrm : NORMAL0;
    float pad : PAD;
//    float2 tex : TEXCOORD0;
    float4 weight : BLENDWEIGHT0;
	
//    float dummy : PADDING; // 明示的に 4B を埋める
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tex : TEXCOORD;
    float4 wpos : TEXCOORD1;
    float4 nrm : TEXCOORD2;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
};

struct LIGHT
{
    float4 Direction; // 方向
    float4 Diffuse; // 拡散反射用の光の強さ
    float4 Ambient; // 環境光用の光の強さ
};

struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Padding;
};

// 定数バッファ受け取り
cbuffer ConstBuffer : register(b0)
{
	// 頂点カラー
    float4 vertexColor;

	// UV座標移動行列
    matrix matrixTex1;

	// ワールド変換行列
    matrix matrixWorld;
	
    bool isInvertX; // ＵＶ座標のⅩを反転するかどうか
	
    int pad[3];
}

cbuffer CameraMatrixBuffer : register(b1)
{
    matrix View3D;
    matrix Projection3D;
    matrix View2D;
    matrix Projection2D;
    matrix ViewSky;
    matrix ProjectionSky;
}

cbuffer LineThicknessBuffer : register(b2)
{
    float lineThickness; // 線の太さ
    float pads[3];
}

cbuffer BlurBuffer : register(b3)
{
    float2 texSize; // テクスチャサイズ
    float2 blurPad; // ブラーをかける方向
}

cbuffer HitFlashBuffer : register(b4)
{
    float3 FlashColor;
    float HitFlashPower; // ヒットフラッシュの強さ 0.0～1.0
    bool IsHitFlash; // ヒットフラッシュを行うかどうか
    float3 _padding;
}

cbuffer LightBuffer : register(b5)
{
    LIGHT Light;
}

cbuffer MaterialBuffer : register(b6)
{
    MATERIAL Material;
}

cbuffer MotionBlurBuffer : register(b7)
{
    // 前フレームのワールド変換行列
    matrix matrixWorldPrev;
    
    // シェルのインデックス（x）、シェルの数（y）、ブラー強度（z）、使用・未使用（w）
    float4 blurParams;
}

cbuffer MotionBlurCircularBuffer : register(b7)
{
    float3 centerPos; // 回転の中心位置
    float lengthBlur; // ブラーの長さ
    
    float4 prevRot; // 前フレームの回転（クォータニオン）
    float4 currentRot; // 今フレームの回転（クォータニオン）
    
    int shellCount; // シェルの数
    float3 padding_3;
}

cbuffer ConstantBufferBoneComb : register(b8)
{
	//	// 頂点カラー
 //   float4 vertexColorB;

	//// UV座標移動行列
 //   matrix matrixTex1B;

	// ワールド変換行列
    matrix matrixWorldB;
	
    matrix bonecombmtx[5]; // ボーンコンビネーション行列
};

cbuffer UIParam : register(b9)
{
    float currentHP;
    float hpScale;
    float2 padding; // 必要なら
};

cbuffer GlowParam : register(b10)
{
    float4 GlowColor; // 発光色（白色）
    float2 EllipseScale; // 楕円スケール
    float GlowPower; // 発光強度（１～５）
    float GlowRadius; // 発光範囲（大きいほど広がる）
    float Angle;
    float3 Pad3;
};

cbuffer GlowParam2 : register(b10)
{
    float4 GlowColor2; // 発光色（白色）
    float2 EllipseScale2; // 楕円スケール
    float GlowPower2; // 発光強度（１～５）
    float GlowRadius2; // 発光範囲（大きいほど広がる）
    float Angle2;
    float RingRadius; // リングの中心半径、0～1.4 ぐらいが目安
    float RingWidth;  // リングの太さ、0.15 ぐらいが良さげ
    float pad1;
};