#include "FighterComponent.h"

FighterComponent::FighterComponent(GameObject& obj) : 	Component(obj)
{
	m_sortNum = FIGHTER;
}

void FighterComponent::Update() {

	if(m_atk < 0) {
		m_atk = 0; // 攻撃力が0以下になったら0にする
	}

	if(m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする

		p_object->SetDeleteFg(true); // オブジェクトを削除フラグを立てる
	}

}