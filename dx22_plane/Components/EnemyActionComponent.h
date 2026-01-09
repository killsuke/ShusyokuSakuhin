#pragma once
#include "Component.h"
#include "FighterComponent.h"
#include "EnemyDeathEventComponent.h"
#include "RightLeft.h"

enum class EEnemyState {
	WAIT,
	MOVE,
	ATTACK,
	DAMAGED,
};

class EnemyActionComponent : public Component
{
protected:
	const float m_deltaTime = 0.016f;
	float m_recordTime = 0.0f;
	RightLeft m_IsRightLeft = RightLeft::RIGHT; // 左右の移動フラグ、初期は右向き(true:右、false:左)
	uint64_t m_listenerID_HitEvent = 0;
	uint64_t m_listenerID_DeathEvent = 0;

	EnemyActionComponent(GameObject& obj);
	~EnemyActionComponent();

public:
	
	virtual void Update() = 0;

	void CreateDamageEffect(const HitEvent& event);
	void ActionOff(const DeathEvent& event);
	RightLeft GetRightLeft()const { return m_IsRightLeft; };
};