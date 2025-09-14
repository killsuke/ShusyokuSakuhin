#pragma once
#include "EnemyActionComponent.h"
class EnemyActionHopperComponent : public EnemyActionComponent
{
private:
	bool m_moveFlag = false; // ˆÚ“®ƒtƒ‰ƒO
public:
	EnemyActionHopperComponent(GameObject& obj);
	~EnemyActionHopperComponent() = default;
	void Update() override;
	void HopperAction(const bool jumpFlag);

};