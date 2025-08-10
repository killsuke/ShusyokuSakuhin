#pragma once
#include "Component.h"
#include <unordered_map>

class AttackComponent : public Component
{
protected:

private:
	float m_coolDownTime = 0.0f;
	std::unordered_map<GameObject*, float> m_attackObjs;

public:
	AttackComponent(GameObject& obj);
	~AttackComponent();

	void Update() override;
	void Attack(GameObject& obj); // UŒ‚ˆ—‚ğs‚¤ŠÖ”i‰¼À‘•j
	inline void SetCoolDownTime(const float time) { m_coolDownTime = time; };
	inline void ClearAttackObjs() { m_attackObjs.clear(); };
};