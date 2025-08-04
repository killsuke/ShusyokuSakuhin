#pragma once
#include "Component.h"
class AttackComponent : public Component
{
protected:

private:
	unsigned int attackTime = 0;

public:
	AttackComponent(GameObject& obj);
	~AttackComponent();

	void Update() override;
	void Attack(GameObject& obj); // UŒ‚ˆ—‚ğs‚¤ŠÖ”i‰¼À‘•j
	inline void SetAttackTime(const int time) { attackTime = time; };
};