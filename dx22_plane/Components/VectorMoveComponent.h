#pragma once
#include "Component.h"
#include <SimpleMath.h>

class VectorMoveComponent :  public Component
{
private:
	DirectX::XMVECTOR m_MoveDirection = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_RotationValue = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_MovePower = 0.0f;

public:

	VectorMoveComponent(GameObject& obj);
	~VectorMoveComponent() = default;
	void Update() override;

	void SetMovePower(const float power) {
		m_MovePower = std::fabsf(power);
	};

	void SetMoveDirection(const DirectX::XMFLOAT3& dir) {
		m_MoveDirection = DirectX::XMLoadFloat3(&dir);
		m_MoveDirection = DirectX::XMVector3Normalize(m_MoveDirection);
	};

	void SetRotationValue(const DirectX::XMFLOAT3& rot) {
		m_RotationValue = rot;
	};
};