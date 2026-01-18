#pragma once
#include "TestSwordActionComponent.h"
#include "MeshCut2DComponent.h"
#include "VectorMoveComponent.h"

struct DeathEvent {
	uint32_t deadID; // 死亡したオブジェクトのインスタンスID
};

enum class EnemyDeathEventState {
	STICKY,
	IMMEDIATE
};

class EnemyDeathEventComponent : public Component
{
private:
	uint64_t m_listenerID_DeathEvent = 0;
	float m_RecordTime = 0.0f;
	EnemyDeathEventState m_State = EnemyDeathEventState::STICKY;
	ESwordActionState m_SwordActionState = ESwordActionState::NONE;
	RightLeft m_RightLeft = RightLeft::RIGHT;

public:
	EnemyDeathEventComponent(GameObject& obj);
	~EnemyDeathEventComponent();
	virtual void Update() override;

	void DeathEventAction(const DeathEvent& event);
	// 切られた状態をセット
	void SetHittedState(const ESwordActionState& state, const RightLeft& dir) {
		m_SwordActionState = state;
		m_RightLeft = dir;
	};

	void SetEnemyDeathEventState(const EnemyDeathEventState& state) {
		m_State = state;
	};

	void ImmediateProcess(MeshCut2DComponent* cutComp, GameObject* obj1, GameObject* obj2);
	void StickyProcess(MeshCut2DComponent* cutComp, GameObject* obj1, GameObject* obj2);
};