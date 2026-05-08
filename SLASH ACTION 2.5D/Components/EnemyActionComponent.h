#pragma once
#include "Component.h"
#include "FighterComponent.h"
#include "EnemyDeathEventComponent.h"
#include "Structs/RightLeft.h"
#include "SoundComponent.h"
#include "Manager/TimeManager.h"

namespace {
	constexpr float FEAR_POWER = 0.8f;
	constexpr float FEAR_SPEED = 50.0f;
	constexpr float FEAR_RIMIT_TIME = 0.8f;
	constexpr float FEAR_DECAY = 0.8f;
}

enum class EEnemyState {
	WAIT,
	MOVE,
	ATTACK,
	DAMAGED,
	DEAD,

	MAX
};

class EnemyActionComponent : public Component
{
protected:
	float m_RecordTime = 0.0f;
	float m_RecordFearTime = 0.0f;
	float m_FearPower = 0.0f; // 怯みの強さ
	bool m_IsFear = false; // 怯み状態かどうかのフラグ
	RightLeft m_IsRightLeft = RightLeft::RIGHT; // 左右の移動フラグ、初期は右向き(true:右、false:左)
	uint64_t m_ListenerID_HitEvent = 0;
	uint64_t m_ListenerID_DeathEvent = 0;
	EEnemyState m_EnemyState = EEnemyState::WAIT;

	EnemyActionComponent(GameObject& obj);
	~EnemyActionComponent();

	void FearAction();	// 怯み状態の処理
	void ChangeState(const EEnemyState& newState) {};	// 状態の変更
	void StateUpdate() {};	// 状態に応じた処理
	virtual void DeadAnimation() {};	// 死亡アニメーションの処理

public:

	virtual void Init() {};
	virtual void Update() = 0;

	void CreateDamageEffect(const HitEvent& event);
	void ActionOff(const DeathEvent& event);
	RightLeft GetRightLeft()const { return m_IsRightLeft; };
};