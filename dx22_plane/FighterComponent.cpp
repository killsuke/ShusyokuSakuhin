#include "FighterComponent.h"

FighterComponent::FighterComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("FIGHTER"); // ソート番号を設定
}

void FighterComponent::Update() {


	if (m_atk < 0) {
		m_atk = 0; // 攻撃力が0以下になったら0にする
	}

	// 合計ダメージを引く
	m_hp -= m_totalDamage;
	m_totalDamage = 0;

	if (m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする

		p_object->SetDeleteFg(true); // オブジェクトを削除フラグを立てる
	}

}