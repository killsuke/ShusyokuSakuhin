#pragma once
#include <SimpleMath.h>
#include	<iostream>
#include	<string.h>
#include	<memory>

// ３Ｄ頂点データ
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Color color = DirectX::SimpleMath::Vector4::Zero;
	DirectX::SimpleMath::Vector2 uv = DirectX::SimpleMath::Vector2::Zero;
};

// 平行光源
struct LIGHT {
	DirectX::SimpleMath::Vector4 Direction = DirectX::SimpleMath::Vector4::Zero;	// 平行光源の方向
	DirectX::SimpleMath::Color Diffuse = DirectX::SimpleMath::Vector4::Zero;		// 平行光源の強さと色
	DirectX::SimpleMath::Color Ambient = DirectX::SimpleMath::Vector4::Zero;		// 環境光の強さと色

};

// サブセット、メッシュ
struct SUBSET {
	std::string  MtrlName = "";			// マテリアル名
	unsigned int IndexNum = 0;		// インデックス数
	unsigned int VertexNum = 0;		// 頂点数
	unsigned int IndexBase = 0;		// 開始インデックス
	unsigned int VertexBase = 0;	// 頂点ベース
	unsigned int MaterialIdx = 0;	// マテリアルの番号

	std::unique_ptr<SUBSET> Clone() const {
		std::unique_ptr<SUBSET> subset = std::make_unique<SUBSET>();
		subset->MtrlName = MtrlName;
		subset->IndexNum = IndexNum;
		subset->VertexNum = VertexNum;
		subset->IndexBase = IndexBase;
		subset->VertexBase = VertexBase;
		subset->MaterialIdx = MaterialIdx;
		return subset;
	}
};

// マテリアル
struct MATERIAL {
	DirectX::SimpleMath::Color Ambient  = DirectX::SimpleMath::Vector4::Zero;	// 環境反射
	DirectX::SimpleMath::Color Diffuse  = DirectX::SimpleMath::Vector4::Zero;	// 拡散反射（ != カラー）
	DirectX::SimpleMath::Color Specular = DirectX::SimpleMath::Vector4::Zero;// 鏡面反射
	DirectX::SimpleMath::Color Emission = DirectX::SimpleMath::Vector4::Zero;// 発光
	float Shiness = 0.0f;	// 光沢の滑らかさ
	BOOL TextureEnable = TRUE; // テクスチャを使うか否かのフラグ
	DirectX::SimpleMath::Vector2 Padding = DirectX::SimpleMath::Vector2::Zero; // パディング

	std::unique_ptr<MATERIAL> Clone() const {
		std::unique_ptr<MATERIAL> material = std::make_unique<MATERIAL>();
		material->Ambient = Ambient;
		material->Diffuse = Diffuse;
		material->Specular = Specular;
		material->Emission = Emission;
		material->Shiness = Shiness;
		material->TextureEnable = TextureEnable;
		material->Padding = Padding;
		return material;
	}
};