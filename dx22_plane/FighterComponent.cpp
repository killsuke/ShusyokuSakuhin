#include "FighterComponent.h"

FighterComponent::FighterComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("FIGHTER"); // ソート番号を設定
}

void FighterComponent::Update() {


	if (m_atk < 0) {
		m_atk = 0; // 攻撃力が0以下になったら0にする
	}

	if (m_totalDamage > 0 && m_invincibleFlag == false) {
		m_invincibleFlag = true; // ダメージを受けたら無敵フラグを立てる
		m_hp -= m_totalDamage;
		m_totalDamage = 0;
	}

	if (m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする
		m_deadFlag = true; // 死亡フラグを立てる
		if (m_useDeadFlag == false) {
			return; // 死亡フラグがfalseなら何もしない
		}
		p_object->SetDeleteFg(true); // オブジェクトを削除フラグを立てる
	}

	if(m_invincibleFlag == true && m_useInvincible == true && m_recordTime < 1.5f) {
		m_recordTime += m_deltaTime;
		return; // 無敵フラグが立っている場合はダメージを受けない
	}
	m_recordTime = 0.0f;
	m_invincibleFlag = false;

	// 合計ダメージを引く
	m_hp -= m_totalDamage;
	m_totalDamage = 0;

	if (m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする
		if (m_useDeadFlag == false) {
			return; // 死亡フラグがfalseなら何もしない
		}
		p_object->SetDeleteFg(true); // オブジェクトを削除フラグを立てる
	}

}