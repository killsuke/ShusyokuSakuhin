#include "AttackTimingComponent.h"
#include "FighterComponent.h"
#include "Collider.h"
#include <iostream>
#include <algorithm>

AttackTimingComponent::AttackTimingComponent(GameObject& obj) : AttackComponent(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("ATTACK"); // ソート番号を設定
}

void AttackTimingComponent::Update() {

	auto collObj = p_object->GetComponent<ColliderComponent>();
	if (collObj->GetActiveColliderFlag() == false) {
		m_attackObjs.clear(); // コリジョンが無効な場合は攻撃オブジェクトをクリア
		return;
	}

	for (auto& attackObj : m_attackObjs) {

		if (attackObj.hitCoolTime > 0.0f) {

			attackObj.hitCoolTime -= 0.016f; // 16msごとに減少
		}
		else {
			attackObj.hitCoolTime = m_coolDownTime; // 0以下になったらリセット
		}
	}

}

void AttackTimingComponent::AttackAction(GameObject& obj) {
	// 攻撃処理の実装
	// ここでは仮の実装として、攻撃力を表示するだけ
	auto fighter = p_object->GetComponent<FighterComponent>();
	auto targetFighter = obj.GetComponent<FighterComponent>();

	if (fighter != nullptr && targetFighter != nullptr) {

		//m_attackHitFlag = false; // 攻撃が当たったフラグを一度リセット

		auto it = std::find_if(m_attackObjs.begin(), m_attackObjs.end(),
			[&obj](const HitRule& hitObj) {return hitObj.target == &obj; });

		if(it != m_attackObjs.end()) {
			if (it->hitCoolTime > 0.0f) {
			//	std::cout << "攻撃クールダウン中..." << std::endl;
				it->hitCount++; // 攻撃が当たった回数を増やす
				return; // 攻撃クールダウン中は何もしない
			}
		}
		else {
			m_attackObjs.push_back({ &obj, m_coolDownTime,1 });	// ここで攻撃対象に組み込む
		}

		m_attackHitFlag = true; // 攻撃が当たったフラグを立てる

		int atk = fighter->GetAtk();

		targetFighter->AddDamage(atk); // 攻撃力分だけ相手のHPを減らす

		//	std::cout << "ここでダメージ！！" << std::endl;
	}
}