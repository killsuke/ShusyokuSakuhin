#pragma once
#include "Component.h"
#include <SimpleMath.h>

class GoAroundComponent : public Component
{
private:
	const float m_deltaTime = 0.016f; // 更新間隔時間
	float m_rotationSpeed = 0.5f; // 回転速度
	float m_angle = 0.0f; // 回転角度
	float m_radius = 10.0f; // 回転半径
	float m_initialAngle = 0.0f;

	GameObject* m_CenterObject = nullptr; // 中心となるオブジェクト
	DirectX::SimpleMath::Vector3 m_initialOffset = {}; // 中心と回転するオブジェクトの差分

public:
	GoAroundComponent(GameObject& obj);
	~GoAroundComponent() = default;

	void Update() override;

	inline void SetCenterObject(GameObject* centerObj) { m_CenterObject = centerObj; }
	inline void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
	inline void SetInitialAngle(float angle) { m_initialAngle = DirectX::XMConvertToRadians(angle); };	// 初期角度をラジアンに変換して設定
	inline void MakeInitialOffset(const DirectX::SimpleMath::Vector3& parent, const DirectX::SimpleMath::Vector3& child) {
		m_initialOffset = child - parent; // 子オブジェクトの位置から親オブジェクトの位置を引いて差分を計算

		m_radius = DirectX::SimpleMath::Vector2(m_initialOffset.x, m_initialOffset.y).Length();
	}
};