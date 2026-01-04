#pragma once
#include "Component.h"

struct DeathEvent {
	uint32_t deadID; // 死亡したオブジェクトのインスタンスID
};

enum class EnemyDeathEventState {
	DELAY,
	IMMEDIATE
};

class EnemyDeathEventComponent : public Component
{
private:
	uint64_t m_listenerID_DeathEvent = 0;
	float m_RecordTime = 0.0f;
	EnemyDeathEventState m_State = EnemyDeathEventState::DELAY;

public:
	EnemyDeathEventComponent(GameObject& obj);
	~EnemyDeathEventComponent();
	virtual void Update() override;

	void DeathEventAction(const DeathEvent& event);

};