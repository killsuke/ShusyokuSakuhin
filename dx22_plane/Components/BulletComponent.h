#pragma once
#include "Component.h"
#include "SimpleMath.h"

class BulletComponent : public Component
{
private:
	DirectX::XMFLOAT3 m_firingVector = DirectX::XMFLOAT3();
	float m_firingSpeed = 0.0f;	// ”ò‚Ô‘¬“x
	const float m_deltaTime = 0.016f;
	float m_rimitTime = 2.0f;

public:
	BulletComponent(GameObject& obj);
	~BulletComponent() = default;

	void Update();

	void SetFiringVector(DirectX::XMFLOAT3 vector) { m_firingVector = vector; };
	void SetFiringSpeed(const float speed) { m_firingSpeed = speed; };
	void SetRimitTime(const float time) { m_rimitTime = time; };
};
