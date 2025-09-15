#pragma once
#include "Component.h"
#include <SimpleMath.h>

class MoveTerrainComponent : public Component
{
private:
	DirectX::SimpleMath::Vector3 m_moveVector = DirectX::SimpleMath::Vector3::Zero;
	float m_moveSpeed = 1.0f;
	const float m_deltaTime = 0.016f;
	float m_recordTime = 0.0f;
	GameObject* m_player = nullptr;
public:
	MoveTerrainComponent(GameObject& obj);
	~MoveTerrainComponent() = default;
	void Update() override;

	void SetMoveVector(DirectX::SimpleMath::Vector3 vector) { m_moveVector = vector; };
	void SetMoveSpeed(const float speed) { m_moveSpeed = speed; };
};