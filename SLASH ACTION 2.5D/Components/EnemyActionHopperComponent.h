#pragma once
#include "EnemyActionComponent.h"

class EnemyActionHopperComponent : public EnemyActionComponent
{
private:
	bool m_MoveFlag = false; // ˆÚ“®ƒtƒ‰ƒO
	uint64_t m_ListenerID_HitEvent_Hopper = 0;

	void ChangeState(const EEnemyState& newState);
	void StateUpdate(GameObject* player);

	void ChangeDirection();

	void DeadAnimation() override;

public:

	EnemyActionHopperComponent(GameObject& obj);
	~EnemyActionHopperComponent();

	void Init() override;
	void Update() override;
	void HopperAction(const bool jumpFlag);

	void KnockBackEvent(const HitEvent& event);
};