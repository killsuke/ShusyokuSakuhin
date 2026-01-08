#pragma once
#include "TestSwordActionComponent.h"
#include "CameraShakeComponent.h"

class EnemyDamageComponent : public Component
{
private:

public:
	EnemyDamageComponent(GameObject& obj);
	~EnemyDamageComponent() = default;

	void Update() override;

	float ChoiceStopTime(const ESwordActionState& state);
	ShakeStatus ChoiceShakeStatus(const ESwordActionState& state);
};
