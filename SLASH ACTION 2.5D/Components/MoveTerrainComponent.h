#pragma once
#include "Component.h"
#include <DirectXMath.h>

class MoveTerrainComponent : public Component
{
private:
	DirectX::XMFLOAT3 m_MoveVector = {};
	DirectX::XMFLOAT3 m_BeforePos = {};
	float m_MoveSpeed = 1.0f;
	float m_RecordTime = 0.0f;
	GameObject* m_player = nullptr;
public:
	MoveTerrainComponent(GameObject& obj);
	~MoveTerrainComponent() = default;
	void Update() override;

	void SetMoveVector(DirectX::XMFLOAT3 vector) { m_MoveVector = vector; };
	void SetMoveSpeed(const float speed) { m_MoveSpeed = speed; };
};