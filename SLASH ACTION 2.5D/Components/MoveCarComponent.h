#pragma once
#include "Component.h"
#include <DirectXMath.h>

class MoveCarComponent : public Component
{
private:
	float m_MoveSpeed = 0.0f;
	DirectX::XMFLOAT3 m_PrevCarPos = {};
	DirectX::XMFLOAT3 m_DeltaCarPos = {};

public:
	MoveCarComponent(GameObject& obj);
	~MoveCarComponent() = default;

	void Update() override;

	void SetMoveSpeed(const float speed) { m_MoveSpeed = speed; };
	float GetMoveSpeed() const { return m_MoveSpeed; };
	DirectX::XMFLOAT3 GetDeltaCarPos() const { return m_DeltaCarPos; };
};