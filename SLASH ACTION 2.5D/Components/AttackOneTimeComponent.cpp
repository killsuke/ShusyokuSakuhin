#include "AttackOneTimeComponent.h"
#include "FighterComponent.h"
#include "ColliderAttackComponent.h"
#include <iostream>

AttackOneTimeComponent::AttackOneTimeComponent(GameObject& obj) : AttackComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ATTACK"); // ソート番号を設定
}

void AttackOneTimeComponent::Update() {

	ColliderAttackComponent* collObj = m_Object->GetComponent<ColliderAttackComponent>();
	
	if(collObj == nullptr) {
		m_AttackObjs.clear(); // コリジョンコンポーネントが存在しない場合は攻撃オブジェクトをクリア
		return;
	}

	if (collObj->GetActiveColliderFlag() == false) {
		m_AttackObjs.clear(); // コリジョンが無効な場合は攻撃オブジェクトをクリア
		return;
	}
}

void AttackOneTimeComponent::AttackAction(GameObject& obj) {

	// 攻撃処理の実装
		// ここでは仮の実装として、攻撃力を表示するだけ
	FighterComponent* fighter = m_Object->GetComponent<FighterComponent>();
	FighterComponent* targetFighter = obj.GetComponent<FighterComponent>();

	if (fighter != nullptr && targetFighter != nullptr) {
		//m_attackHitFlag = false; // 攻撃が当たったフラグを一度リセット

		std::vector<HitRule>::iterator it = std::find_if(m_AttackObjs.begin(), m_AttackObjs.end(),
			[&obj](const HitRule& hitObj) {return hitObj.target == &obj; });

		if (it != m_AttackObjs.end()) {
			if (it->hitCount > 0) {

				it->hitCount++; // 攻撃が当たった回数を増やす
				return; // 同じ敵には何もしない
			}
		}
		else {
			m_AttackObjs.push_back({ &obj, 0.0f,1 });	// ここで攻撃対象に組み込む
		}

		m_AttackHitFlag = true; // 攻撃が当たったフラグを立てる

		const int atk = fighter->GetAtk();

		targetFighter->AddDamage(atk); // 攻撃力分だけ相手のHPを減らす

		//	std::cout << "ここでダメージ！！" << std::endl;
	}
}