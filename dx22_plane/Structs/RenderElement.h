#pragma once
#include <DirectXMath.h>
#include	<iostream>
#include	<string.h>
#include	<memory>

// ３Ｄ頂点データ
struct VERTEX_3D
{
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3();
	DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3();
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4();
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2();
};

// 平行光源
struct LIGHT {
	DirectX::XMFLOAT4 Direction = DirectX::XMFLOAT4();	// 平行光源の方向
	DirectX::XMFLOAT4 Diffuse = DirectX::XMFLOAT4();		// 平行光源の強さと色
	DirectX::XMFLOAT4 Ambient = DirectX::XMFLOAT4();		// 環境光の強さと色

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
	DirectX::XMFLOAT4 Ambient  = DirectX::XMFLOAT4();	// 環境反射
	DirectX::XMFLOAT4 Diffuse  = DirectX::XMFLOAT4();	// 拡散反射（ != カラー）
	DirectX::XMFLOAT4 Specular = DirectX::XMFLOAT4();// 鏡面反射
	DirectX::XMFLOAT4 Emission = DirectX::XMFLOAT4();// 発光
	float Shiness = 0.0f;	// 光沢の滑らかさ
	BOOL TextureEnable = TRUE; // テクスチャを使うか否かのフラグ
	DirectX::XMFLOAT2 Padding = DirectX::XMFLOAT2(); // パディング

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