#pragma once
#include <string>
#include <DirectXMath.h>

struct TerrainStatus {
	std::string kind = "";	// 地形の種類（例：草地、砂地、岩場など）
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3();	// 地形のサイズ
	DirectX::XMFLOAT3 angle = DirectX::XMFLOAT3();	// 地形の角度
	std::string texture = "";	// テクスチャのパス
	std::string shaderVS = "";	// 頂点シェーダーのパス
	std::string shaderPS = "";	// ピクセルシェーダーのパス
};

struct EnemyStatus {
	std::string kind = "";	// 敵の種類（例：ゴブリン、ドラゴンなど）
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3();	// 地形のサイズ
	DirectX::XMFLOAT3 angle = DirectX::XMFLOAT3();	// 地形の角度
	int atk = 0;
	int hp = 0;
	std::string texture = "";	// テクスチャのパス
	std::string shaderVS = "";	// 頂点シェーダーのパス
	std::string shaderPS = "";	// ピクセルシェーダーのパス
};