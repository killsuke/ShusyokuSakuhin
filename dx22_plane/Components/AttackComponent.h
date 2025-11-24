#pragma once
#include "Component.h"
#include <vector>

struct HitRule {
	GameObject* target = nullptr; // 攻撃対象のゲームオブジェクト
	float hitCoolTime = 0.0f; // 攻撃が当たったクールダウン時間
	int hitCount = 0; // 攻撃が当たった回数
};

class AttackComponent : public Component
{
protected:
	AttackComponent(GameObject& obj);
	~AttackComponent() = default;

	std::vector<HitRule> m_attackObjs;
	bool m_attackHitFlag = false; // 攻撃が当たったかどうかのフラグ
public:
	
	void Update() override;

	virtual void AttackAction(GameObject& obj) = 0; // 攻撃処理を行う関数

	inline void ClearAttackObjs() { m_attackObjs.clear(); };

	inline void ReSetAttackHitFlag() { m_attackHitFlag = false; };
	inline bool GetAttackHitFlag() const { return m_attackHitFlag; };
};
