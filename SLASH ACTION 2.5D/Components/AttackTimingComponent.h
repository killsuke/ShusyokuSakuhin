#pragma once
#include "AttackComponent.h"

class AttackTimingComponent : public AttackComponent
{
private:
	float m_CoolDownTime = 0.0f;

public:
	AttackTimingComponent(GameObject& obj);
	~AttackTimingComponent() = default;

	void Update() override;
	void SetCoolDownTime(const float time) { m_CoolDownTime = time; };
	void AttackAction(GameObject& obj) override;
};