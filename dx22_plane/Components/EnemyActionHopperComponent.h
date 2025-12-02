#pragma once
#include "EnemyActionComponent.h"
class EnemyActionHopperComponent : public EnemyActionComponent
{
private:
	bool m_moveFlag = false; // ˆÚ“®ƒtƒ‰ƒO
	bool m_IsBeforeJump = false;

public:
	EnemyActionHopperComponent(GameObject& obj);
	~EnemyActionHopperComponent() = default;
	void Update() override;
	void HopperAction(const bool jumpFlag);

};