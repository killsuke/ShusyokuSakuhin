#pragma once
#include "EnemyActionComponent.h"
class EnemyActionBulletComponent final : public EnemyActionComponent
{
private:


public:
	EnemyActionBulletComponent(GameObject& obj);
	~EnemyActionBulletComponent() = default;

	void Update()override;
	void FiringBullet();

};

