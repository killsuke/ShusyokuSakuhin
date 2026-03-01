#pragma once
#include "EnemyActionComponent.h"

class PlayerDamageComponent : public Component
{
private:
	EnemyActionComponent* m_EnemyActionComp = nullptr;

public:
	PlayerDamageComponent(GameObject& obj);
	~PlayerDamageComponent() = default;
	void Update() override;

	void SetEnemyActionComponent(EnemyActionComponent* comp) { m_EnemyActionComp = comp; };
};

