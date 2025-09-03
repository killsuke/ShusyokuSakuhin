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
}

cbuffer ViewBuffer3D : register(b1)
{
    matrix View3D;
}
cbuffer ProjectionBuffer3D : register(b2)
{
    matrix Projection3D;
}

cbuffer ViewBuffer2D : register(b3)
{
    matrix View2D;
}
cbuffer ProjectionBuffer2D : register(b4)
{
    matrix Projection2D;
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
    float4 col : COLOR0;
    int4 idx : BLENDINDICES;

    float3 pos : POSITION;
//    float4 nrm : NORMAL0;
	//float pad : PAD;
//    float2 tex : TEXCOORD0;
    float3 weight : BLENDWEIGHT;
	
    float2 dummy : PADDING; // 明示的に 4B を埋める
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
	bool   Enable;		// 使用するか否か
	bool3  Dummy;		// PADDING
	float4 Direction;	// 方向
	float4 Diffuse;		// 拡散反射用の光の強さ
	float4 Ambient;		// 環境光用の光の強さ
};

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
	bool2 Dummy;
};

cbuffer MaterialBuffer:register(b6) {
	MATERIAL Material;
}

// ＵＶ座標移動行列
cbuffer MaterialTexBuffer : register(b6) {
	matrix matrixTex;
}

cbuffer ConstantBufferBoneComb : register(b8)
{	
	//	// 頂点カラー
 //   float4 vertexColorB;

	//// UV座標移動行列
 //   matrix matrixTex1B;

	// ワールド変換行列
    matrix matrixWorldB;
	
    matrix bonecombmtx[2]; // ボーンコンビネーション行列
};

cbuffer UIParam : register(b9)
{
    float currentHP;
    float hpScale;
    float2 padding; // 必要なら
};

cbuffer ViewBufferSky : register(b10)
{
    matrix ViewSky;
}
cbuffer ProjectionBufferSky : register(b11)
{
    matrix ProjectionSky;
}