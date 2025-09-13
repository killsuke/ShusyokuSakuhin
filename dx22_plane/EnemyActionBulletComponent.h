#pragma once
#include "EnemyActionComponent.h"
class EnemyActionBulletComponent : public EnemyActionComponent
{
private:


public:
	EnemyActionBulletComponent(GameObject& obj);
	~EnemyActionBulletComponent() = default;

	void Update();
	void FiringBullet();

};

