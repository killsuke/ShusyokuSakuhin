#include "AttackComponent.h"
#include "FighterComponent.h"
#include <iostream>

AttackComponent::AttackComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ATTACK;
	attackTime = 10;
}

AttackComponent::~AttackComponent() {

}

void AttackComponent::Update() {
	if (attackTime > 0) {
		attackTime--;

	}

	if (attackTime < 1) {
		attackTime = 0;
	}
}

void AttackComponent::Attack(GameObject& obj) {
	// 攻撃処理の実装
	// ここでは仮の実装として、攻撃力を表示するだけ
	auto fighter = p_object->GetComponent<FighterComponent>();
	auto targetFighter = obj.GetComponent<FighterComponent>();

	if (attackTime == 0 && fighter != nullptr && targetFighter != nullptr) {
		int atk = fighter->GetAtk();
		int targetHp = targetFighter->GetHp();

		targetFighter->AddHp(-atk); // 攻撃力分だけ相手のHPを減らす

		attackTime = 10;

		std::cout << "10ダメージ！！" << std::endl;
	}
}