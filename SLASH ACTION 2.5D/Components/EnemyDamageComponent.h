#pragma once
#include "TestSwordActionComponent.h"
#include "CameraShakeComponent.h"

class EnemyDamageComponent : public Component
{
private:
	ShakeStatus ChoiceShakeStatus(const ESwordActionState& state);
	float ChoiceStopTime(const ESwordActionState& state);

public:
	EnemyDamageComponent(GameObject& obj);
	~EnemyDamageComponent() = default;

	void Update() override;
};
