//cbuffer WorldBuffer : register(b0)
//{
//	matrix World;
//}

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

struct LIGHT {
	float4 Direction;	// 方向
	float4 Diffuse;		// 拡散反射用の光の強さ
	float4 Ambient;		// 環境光用の光の強さ
};

cbuffer HitFlashBuffer:register(b4) {
    float3 FlashColor;
	float HitFlashPower; // ヒットフラッシュの強さ 0.0～1.0
    bool IsHitFlash; // ヒットフラッシュを行うかどうか
    float3 _padding;
}

cbuffer LightBuffer:register(b5) {
	LIGHT Light;
}

struct MATERIAL {
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
    bool TextureEnable;
    float2 Padding;
};

cbuffer MaterialBuffer:register(b6) {
	MATERIAL Material;
}

cbuffer MotionBlurBuffer : register(b7)
{
    float2 velocity; // ピクセル単位の速度（前フレーム座標との差分など）
    float2 pad2;
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