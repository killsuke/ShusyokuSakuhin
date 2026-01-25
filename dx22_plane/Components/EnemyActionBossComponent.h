#pragma once
#include "EnemyActionComponent.h"

enum class BossActionState {
	DEFAULT,
	JUMP_SHOOTING,
	BARRIER,
	MAX
};

class EnemyActionBossComponent : public EnemyActionComponent
{
private:
	BossActionState m_CurrentState = BossActionState::DEFAULT;
	float m_RecordTime1 = 0.0f;
	float m_MoveDir = 1.0f;

public:
	EnemyActionBossComponent(GameObject& obj);
	~EnemyActionBossComponent() = default;
	void Update()override;
	void BossAction(const DirectX::XMFLOAT3& playPos, const DirectX::XMFLOAT3& myPos);
	void ChangeState(const BossActionState& state);
};