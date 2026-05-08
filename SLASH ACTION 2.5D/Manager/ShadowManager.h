#pragma once

#include "GameObjectManager.h"
#include "Components/TransformComponent.h"
#include <array>
#include <vector>

namespace {

	constexpr size_t SHADOW_OBJECT_MAX = 32; // シャドウオブジェクトの最大数
	constexpr size_t ENEMY_MAX = SHADOW_OBJECT_MAX - 1; // 敵の最大数（プレイヤーの分を残しておく）
	constexpr float SHADOW_RADIUS_MULTIPLIER_PLAYER = 2.5f; // シャドウの半径をオブジェクトのスケールに対してどれだけ大きくするかの倍率
	constexpr float SHADOW_RADIUS_MULTIPLIER_ENEMY = 1.5f; // シャドウの半径をオブジェクトのスケールに対してどれだけ大きくするかの倍率
}

struct ShadowData {

	DirectX::XMFLOAT3 objectPos = DirectX::XMFLOAT3();	// 影を出すオブジェクトの位置
	float shadowRadius = 0.0f;							// 影の半径
};

struct ShadowBuffer {

	std::array<ShadowData, SHADOW_OBJECT_MAX> shadowData = {};	// 影を出すオブジェクトの位置と半径の配列
	int shadowCount = 0;
	DirectX::XMFLOAT3 padding = DirectX::XMFLOAT3(); // 4バイトのパディング
};

class ShadowManager final
{
private:

	static inline std::array<ShadowData, SHADOW_OBJECT_MAX> m_ShadowBuffers; // シャドウバッファの配列
	static inline int m_ShadowCount = 0; // 現在のシャドウオブジェクトの数

	// コンストラクタ・デストラクタを削除
	ShadowManager() = delete;
	~ShadowManager() = delete;

	// コピー・ムーブも削除
	ShadowManager(const ShadowManager&) = delete;
	ShadowManager(ShadowManager&&) = delete;
	ShadowManager& operator=(const ShadowManager&) = delete;
	ShadowManager& operator=(ShadowManager&&) = delete;

public:

	static void Update();

	static const std::array<ShadowData, SHADOW_OBJECT_MAX>& GetShadowBuffers() {
		return m_ShadowBuffers;
	}
};