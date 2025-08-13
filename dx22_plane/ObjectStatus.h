#pragma once
#include <string>
#include <SimpleMath.h>

struct TerrainStatus {
	std::string kind;	// 地形の種類（例：草地、砂地、岩場など）
	DirectX::SimpleMath::Vector3 scale;	// 地形のサイズ
	DirectX::SimpleMath::Vector3 angle;	// 地形の角度
	std::string texture;	// テクスチャのパス
	std::string shader;	// シェーダーのパス
};

struct EnemyStatus {
	std::string kind;	// 敵の種類（例：ゴブリン、ドラゴンなど）
	DirectX::SimpleMath::Vector3 scale;	// 地形のサイズ
	DirectX::SimpleMath::Vector3 angle;	// 地形の角度
	std::string texture;	// テクスチャのパス
	std::string shader;	// シェーダーのパス
};