#pragma once
#include "EnemyActionComponent.h"

class EnemyActionBulletComponent : public EnemyActionComponent
{
private:

	uint64_t m_listenerID_HitEvent_Bullet = 0;

	void FearEvent(const HitEvent& event);

public:
	EnemyActionBulletComponent(GameObject& obj);
	~EnemyActionBulletComponent();

	void Update()override;
	void FiringBullet();

};

