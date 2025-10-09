#pragma once
#include "AttackComponent.h"

class AttackOneTimeComponent final : public AttackComponent
{
private:

public:
	AttackOneTimeComponent(GameObject& obj);
	~AttackOneTimeComponent() = default;

	void Update() override;
	void AttackAction(GameObject& obj) override;
};