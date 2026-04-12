#pragma once
#include "Component.h"

class DeadRingComponent : public Component
{
private:
	float m_ScaleUpSpeed = 0.0f; // 拡大するスピード
	float m_DeleteSpeed = 0.0f; // 消えるスピード

public:

	DeadRingComponent(GameObject& obj);
	~DeadRingComponent() = default;

	void Update() override;
	void SetScaleUpSpeed(const float speed) { m_ScaleUpSpeed = speed; };
	void SetDeleteSpeed(const float speed) { m_DeleteSpeed = speed; };
};