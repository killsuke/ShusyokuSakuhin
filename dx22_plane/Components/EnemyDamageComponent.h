#pragma once
#include "Component.h"
#include "TestSwordActionComponent.h"

class EnemyDamageComponent : public Component
{
private:

public:
	EnemyDamageComponent(GameObject& obj);
	~EnemyDamageComponent() = default;

	void Update() override;

	float ChoiceStopTime(const ESwordActionState state);
};
