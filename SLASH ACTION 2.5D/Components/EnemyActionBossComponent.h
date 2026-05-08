#pragma once
#include "EnemyActionComponent.h"
#include <array>

namespace{
	
	constexpr unsigned int BarrierCount = 4;
	constexpr unsigned int ShurikenCount = 4;
}

enum class BossActionState {

	DEFAULT,
	JUMP_SHOOTING,
	BARRIER,
	DAMAGE,
	MAX
};

class EnemyActionBossComponent : public EnemyActionComponent
{
private:
	std::array<GameObject*, BarrierCount> m_BarrierList;
	std::array<GameObject*, ShurikenCount> m_ShurikenList;
	GameObject* m_BossBarrier = nullptr;
	GameObject* m_HpBar = nullptr;
	BossActionState m_CurrentState = BossActionState::DEFAULT;
	uint64_t m_ListenerID_DamageEvent = 0;
	float m_RecordTime_Jump = 0.0f;
	float m_RecordTime_Damage = 0.0f;
	float m_MoveDir = 1.0f;
	float m_LengthCount = 0.0f;		// ジャンプ攻撃でプレイヤーとの距離を測るためのカウンター
	int m_ShurikenCount = 0;		// ジャンプ攻撃で撃つ手裏剣の数
	bool m_IsBarrier = false;

	void JumpBullet(const DirectX::XMFLOAT3& playPos, const DirectX::XMFLOAT3& myPos);
	void ChangeState(const BossActionState& state);
	void ResetBarriers(FighterComponent& fight);
	void DeadAnimation() override;
	void BarrierDurabilityCheck();
	void StateUpdate(const DirectX::XMFLOAT3& myPos, const DirectX::XMFLOAT3& playPos, const bool isGround);
	void DamageEvent(const HitEvent& event);

public:

	EnemyActionBossComponent(GameObject& obj);
	~EnemyActionBossComponent() = default;
	
	void Init() override;
	void Update()override;
};