#pragma once
#include "TestSwordActionComponent.h"
#include "MeshCut2DComponent.h"
#include "VectorMoveComponent.h"
#include "TransformComponent.h"
#include <vector>

struct DeathEvent {
	uint32_t deadID; // 死亡したオブジェクトのインスタンスID
};

enum class EnemyDeathEventState {
	STICKY,		// 貼りつき
	IMMEDIATE	// 飛んで来るだけ
};

enum class DeathPattern {
	NONE = -1,
	DEFAULT,
	CHARGE,

	MAX
};

enum DeathType {

	RAMDOM,
	ABS_STICKY,
	ABS_IMMEDIATE,

	MAX
};

class EnemyDeathEventComponent : public Component
{
private:
	uint64_t m_ListenerID_DeathEvent = 0;

	EnemyDeathEventState m_DeathState = EnemyDeathEventState::STICKY;
	ESwordActionState m_SwordActionState = ESwordActionState::NONE;
	RightLeft m_RightLeft = RightLeft::RIGHT;

	DeathPattern m_DeathPattern = DeathPattern::NONE;
	DeathType m_DeathType = DeathType::RAMDOM;

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
		m_DeathState = state;
	};

	
	void SetDeathPattern(const DeathPattern pattern) {

		m_DeathPattern = pattern;

		if (m_DeathPattern == DeathPattern::DEFAULT) {
			m_DeathState = EnemyDeathEventState::IMMEDIATE;
		}
		else if (m_DeathPattern == DeathPattern::CHARGE) {
			m_DeathState = EnemyDeathEventState::STICKY;
		}
	};

	void SetDeathType(const DeathType type) {
		m_DeathType = type;
	};
};