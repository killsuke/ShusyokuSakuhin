#pragma once
#include <SimpleMath.h>
#include	<iostream>
#include	<string.h>

// ３Ｄ頂点データ
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Color color;
	DirectX::SimpleMath::Vector2 uv;
};

// 平行光源
struct LIGHT {
	DirectX::SimpleMath::Vector4 Direction;	// 平行光源の方向
	DirectX::SimpleMath::Color Diffuse;		// 平行光源の強さと色
	DirectX::SimpleMath::Color Ambient;		// 環境光の強さと色

};

// サブセット、メッシュ
struct SUBSET {
	std::string  MtrlName;			// マテリアル名
	unsigned int IndexNum = 0;		// インデックス数
	unsigned int VertexNum = 0;		// 頂点数
	unsigned int IndexBase = 0;		// 開始インデックス
	unsigned int VertexBase = 0;	// 頂点ベース
	unsigned int MaterialIdx = 0;	// マテリアルの番号
};

// マテリアル
struct MATERIAL {
	DirectX::SimpleMath::Color Ambient;	// 環境反射
	DirectX::SimpleMath::Color Diffuse;	// 拡散反射（ != カラー）
	DirectX::SimpleMath::Color Specular;// 鏡面反射
	DirectX::SimpleMath::Color Emission;// 発光
	float Shiness;	// 光沢の滑らかさ
	BOOL TextureEnable; // テクスチャを使うか否かのフラグ
	BOOL Dummy[2];
};