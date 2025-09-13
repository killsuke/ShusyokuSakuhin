#pragma once
#include "EnemyActionComponent.h"
class EnemyActionHopperComponent : public EnemyActionComponent
{
private:

public:
	EnemyActionHopperComponent(GameObject& obj);
	~EnemyActionHopperComponent() = default;
	void Update() override;
	void HopperAction(const bool jumpFlag);

};