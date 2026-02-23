#pragma once
#include "Component.h"
#include <DirectXMath.h>

class FliesMoveComponent : public Component
{
private:
	DirectX::XMVECTOR m_MoveDirection = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR m_TargetDirection = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float m_MovePower = 0.0f;
	float m_ChangeTargetTime = 0.0f;

public:
	FliesMoveComponent(GameObject& obj);
	~FliesMoveComponent() = default;
	void Update()override;

	void SetMovePower(const float power) {
		m_MovePower = std::fabsf(power);
	};
};