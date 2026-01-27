#pragma once
#include "EnemyActionComponent.h"
#include <array>

namespace{
	
	constexpr unsigned int BarrierCount = 4;
}

enum class BossActionState {
	DEFAULT,
	JUMP_SHOOTING,
	BARRIER,
	MAX
};

class EnemyActionBossComponent : public EnemyActionComponent
{
private:
	std::array<GameObject*, BarrierCount> m_BarrierList;
	GameObject* m_BossBarrier = nullptr;
	GameObject* m_HpBar = nullptr;
	BossActionState m_CurrentState = BossActionState::DEFAULT;
	float m_RecordTime1 = 0.0f;
	float m_MoveDir = 1.0f;
	float m_LengthCount = 0.0f;
	bool m_IsBarrier = false;

	void JumpBullet(const DirectX::XMFLOAT3& playPos, const DirectX::XMFLOAT3& myPos);
	void ChangeState(const BossActionState& state);
	void ResetBarriers(FighterComponent& fight);

public:
	EnemyActionBossComponent(GameObject& obj);
	~EnemyActionBossComponent() = default;
	void Update()override;
};