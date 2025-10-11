#pragma once
#include "EnemyActionComponent.h"
class EnemyActionBossComponent : public EnemyActionComponent
{
private:

public:
	EnemyActionBossComponent(GameObject& obj);
	~EnemyActionBossComponent() = default;
	void Update()override;
	void BossAction();
};