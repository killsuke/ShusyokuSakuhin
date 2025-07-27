//cbuffer WorldBuffer : register(b0)
//{
//	matrix World;
//}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

// 定数バッファ受け取り
cbuffer ConstBuffer : register(b0) 
{
	// 頂点カラー
	float4 vertexColor;

	// UV座標移動行列
	matrix matrixTex1;

	// プロジェクション変換行列
	matrix matrixProj;

	// ワールド変換行列
	matrix matrixWorld;

	// ビュー変換行列
	matrix matrixView;
}

struct VS_IN
{
    float4 pos : POSITION0;
	float4 nrm : NORMAL0;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
};

struct VS_ANIMATION
{
    float4 pos : POSITION0;
    float4 nrm : NORMAL0;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
    float3 weight : BLENDWEIGHT0;
    int4 idx : BLENDINDICES0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 nrm : TEXCOORD0;
    float4 col : COLOR0;
    float2 tex : TEXCOORD1;
    float4 wpos : TEXCOORD2;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct LIGHT {
	bool   Enable;		// 使用するか否か
	bool3  Dummy;		// PADDING
	float4 Direction;	// 方向
	float4 Diffuse;		// 拡散反射用の光の強さ
	float4 Ambient;		// 環境光用の光の強さ
};

cbuffer LightBuffer:register(b3) {
	LIGHT Light;
}

struct MATERIAL {
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	bool2 Dummy;
};

cbuffer MaterialBuffer:register(b4) {
	MATERIAL Material;
}

// ＵＶ座標移動行列
cbuffer MaterialBuffer : register(b5) {
	matrix matrixTex;
}

cbuffer ConstantBufferBoneComb : register(b6)
{
    matrix bonecombmtx[7]; // ボーンコンビネーション行列
};