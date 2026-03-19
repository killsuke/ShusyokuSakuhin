#pragma once
#include "EnemyActionComponent.h"

class EnemyActionHopperComponent : public EnemyActionComponent
{
private:
	bool m_moveFlag = false; // ˆÚ“®ƒtƒ‰ƒO
	bool m_IsBeforeJump = false;
	uint64_t m_listenerID_HitEvent_Hopper = 0;

	void DeadAnimation() override {};

public:
	
	EnemyActionHopperComponent(GameObject& obj);
	~EnemyActionHopperComponent();
	
	void Init() override;
	void Update() override;
	void HopperAction(const bool jumpFlag);

	void KnockBackEvent(const HitEvent& event);
};