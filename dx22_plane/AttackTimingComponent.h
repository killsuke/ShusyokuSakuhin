#pragma once
#include "AttackComponent.h"

class AttackTimingComponent final : public AttackComponent
{
private:
	float m_coolDownTime = 0.0f;

public:
	AttackTimingComponent(GameObject& obj);
	~AttackTimingComponent() = default;

	void Update() override;
	inline void SetCoolDownTime(const float time) { m_coolDownTime = time; };
	void AttackAction(GameObject& obj) override;
};