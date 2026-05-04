#include "AttackTimingComponent.h"
#include "FighterComponent.h"
#include "ColliderAttackComponent.h"
#include "Manager/TimeManager.h"
#include <iostream>
#include <algorithm>

AttackTimingComponent::AttackTimingComponent(GameObject& obj) : AttackComponent(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("ATTACK"); // ソート番号を設定
}

void AttackTimingComponent::Update() {

	ColliderAttackComponent* collObj = m_Object->GetComponent<ColliderAttackComponent>();
	if(collObj == nullptr) {
		return; // コリジョンコンポーネントがない場合は何もしない
	}

	if (collObj->GetActiveColliderFlag() == false) {
		m_AttackObjs.clear(); // コリジョンが無効な場合は攻撃オブジェクトをクリア
		return;
	}

	for (HitRule& attackObj : m_AttackObjs) {

		if (attackObj.hitCoolTime > 0.0f) {

			const float deltaTime = TimeManager::GetFixedDeltaTime();

			attackObj.hitCoolTime -= deltaTime; // 固定フレームレートのデルタタイムを使用
		}
		else {
			attackObj.hitCoolTime = m_coolDownTime; // 0以下になったらリセット
		}
	}

}

void AttackTimingComponent::AttackAction(GameObject& obj) {
	// 攻撃処理の実装
	// ここでは仮の実装として、攻撃力を表示するだけ
	FighterComponent* fighter = m_Object->GetComponent<FighterComponent>();
	FighterComponent* targetFighter = obj.GetComponent<FighterComponent>();

	if (fighter != nullptr && targetFighter != nullptr) {

		//m_attackHitFlag = false; // 攻撃が当たったフラグを一度リセット

		std::vector<HitRule>::iterator it = std::find_if(m_AttackObjs.begin(), m_AttackObjs.end(),
			[&obj](const HitRule& hitObj) {return hitObj.target == &obj; });

		if(it != m_AttackObjs.end()) {
			if (it->hitCoolTime > 0.0f) {
			//	std::cout << "攻撃クールダウン中..." << std::endl;
				it->hitCount++; // 攻撃が当たった回数を増やす
				return; // 攻撃クールダウン中は何もしない
			}
		}
		else {
			m_AttackObjs.push_back({ &obj, m_coolDownTime,1 });	// ここで攻撃対象に組み込む
		}

		m_AttackHitFlag = true; // 攻撃が当たったフラグを立てる

		int atk = fighter->GetAtk();

		targetFighter->AddDamage(atk); // 攻撃力分だけ相手のHPを減らす

		//	std::cout << "ここでダメージ！！" << std::endl;
	}
}