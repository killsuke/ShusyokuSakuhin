#pragma once
#include "SystemBase.h"
#include <SimpleMath.h>
#include <map>
#include <iostream>

struct TransformComponent {
	// SRT情報（姿勢情報）
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// SRT情報（姿勢情報）
	DirectX::SimpleMath::Vector3 m_LocalPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_LocalRotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_LocalScale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// ワールド行列
	DirectX::SimpleMath::Matrix worldMatrix = {};
};

class TransformSystem : public SystemBase
{
private:
	std::unordered_map<uint32_t, TransformComponent> components;	// エンティティ保存用
public:
	TransformSystem() = default;

	~TransformSystem() = default;

	// ベクター内にエンティティ追加
	void AddComponent(uint32_t entity, const TransformComponent& comp);

	// エンティティからコンポーネントを削除
	void RemoveComponent(uint32_t entity)override;

	// トランスフォームのコンポーネント取得
	TransformComponent* GetComponent(uint32_t entity);

	// 更新処理
	void Update()override;
};

