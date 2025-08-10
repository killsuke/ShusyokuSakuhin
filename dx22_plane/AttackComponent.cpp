#include "AttackComponent.h"
#include "FighterComponent.h"
#include "Collider.h"
#include <iostream>

AttackComponent::AttackComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ATTACK;
}

AttackComponent::~AttackComponent() {

}

void AttackComponent::Update() {

	auto collObj = p_object->GetComponent<ColliderComponent>();
	if (collObj->GetActiveFlag() == false) {
		m_attackObjs.clear(); // コリジョンが無効な場合は攻撃オブジェクトをクリア
		return;
	}

	for(auto& attackObj : m_attackObjs) {
		if (attackObj.second > 0.0f) {
			attackObj.second -= 0.016f; // 16msごとに減少
		}
		else {
			attackObj.second = m_coolDownTime; // 0以下になったらリセット
		}
	}

}

void AttackComponent::Attack(GameObject& obj) {
	// 攻撃処理の実装
	// ここでは仮の実装として、攻撃力を表示するだけ
	auto fighter = p_object->GetComponent<FighterComponent>();
	auto targetFighter = obj.GetComponent<FighterComponent>();

	if (fighter != nullptr && targetFighter != nullptr) {
		
		auto it = m_attackObjs.find(&obj);

		if(it != m_attackObjs.end()) {
			if (it->second > 0.0f) {
			//	std::cout << "攻撃クールダウン中..." << std::endl;
				return; // 攻撃クールダウン中は何もしない
			}
		}
		else {
			m_attackObjs.insert({ &obj, m_coolDownTime });	// ここで攻撃対象に組み込む
		}

		int atk = fighter->GetAtk();

		targetFighter->AddDamage(atk); // 攻撃力分だけ相手のHPを減らす

		std::cout << "ここでダメージ！！" << std::endl;
	}
}