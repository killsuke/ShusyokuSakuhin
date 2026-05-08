#pragma once
#include "AttackComponent.h"

class AttackOneTimeComponent : public AttackComponent
{
public:
	AttackOneTimeComponent(GameObject& obj);
	~AttackOneTimeComponent() = default;

	void Update() override;
	void AttackAction(GameObject& obj) override;
};