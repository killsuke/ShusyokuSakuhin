#pragma once
#include "Component.h"
#include "SimpleMath.h"

class BulletComponent : public Component
{
private:
	DirectX::SimpleMath::Vector3 m_firingVector = DirectX::SimpleMath::Vector3::Zero;
	float m_firingSpeed = 0.0f;
	const float m_deltaTime = 0.016f;
	float m_rimitTime = 2.0f;

public:
	BulletComponent(GameObject& obj);
	~BulletComponent() = default;

	void Update();

	void SetFiringVector(DirectX::SimpleMath::Vector3 vector) { m_firingVector = vector; };
	void SetFiringSpeed(const float speed) { m_firingSpeed = speed; };
};
