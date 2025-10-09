#pragma once
#include "Component.h"
class EnemyDamageComponent final : public Component
{
private:

public:
	EnemyDamageComponent(GameObject& obj);
	~EnemyDamageComponent() = default;

	void Update() override;
};
