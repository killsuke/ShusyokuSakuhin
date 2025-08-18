#include "AttackOneTimeComponent.h"
#include "Collider.h"
#include "FighterComponent.h"
#include <iostream>

AttackOneTimeComponent::AttackOneTimeComponent(GameObject& obj) : AttackComponent(obj) {
	m_sortNum = ATTACK; // ソート番号を設定
}

void AttackOneTimeComponent::Update() {
	auto collObj = p_object->GetComponent<ColliderComponent>();
	if (collObj->GetActiveColliderFlag() == false) {
		m_attackObjs.clear(); // コリジョンが無効な場合は攻撃オブジェクトをクリア
		return;
	}

}

void AttackOneTimeComponent::AttackAction(GameObject& obj) {

	// 攻撃処理の実装
		// ここでは仮の実装として、攻撃力を表示するだけ
	auto fighter = p_object->GetComponent<FighterComponent>();
	auto targetFighter = obj.GetComponent<FighterComponent>();

	if (fighter != nullptr && targetFighter != nullptr) {

		auto it = std::find_if(m_attackObjs.begin(), m_attackObjs.end(),
			[&obj](const HitRule& hitObj) {return hitObj.target == &obj; });

		if (it != m_attackObjs.end()) {
			if (it->hitCount > 0) {

				it->hitCount++; // 攻撃が当たった回数を増やす
				return; // 同じ敵には何もしない
			}
		}
		else {
			m_attackObjs.push_back({ &obj, 0.0f,1 });	// ここで攻撃対象に組み込む
		}

		int atk = fighter->GetAtk();

		targetFighter->AddDamage(atk); // 攻撃力分だけ相手のHPを減らす

			std::cout << "ここでダメージ！！" << std::endl;
	}
}