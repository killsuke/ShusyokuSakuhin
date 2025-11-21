#pragma once
#include "Component.h"
#include <SimpleMath.h>

class MoveCarComponent : public Component
{
private:
	float m_MoveSpeed = 0.0f;
	DirectX::SimpleMath::Vector3 m_PrevCarPos = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 m_DeltaCarPos = DirectX::SimpleMath::Vector3::Zero;

public:
	MoveCarComponent(GameObject& obj);
	~MoveCarComponent() = default;

	void Update() override;

	void SetMoveSpeed(const float speed) { m_MoveSpeed = speed; };
	float GetMoveSpeed() const { return m_MoveSpeed; };
	DirectX::SimpleMath::Vector3 GetDeltaCarPos() const { return m_DeltaCarPos; };
};