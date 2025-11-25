#include "FighterComponent.h"
#include "GameObjectManager.h"

FighterComponent::FighterComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("FIGHTER"); // ソート番号を設定
}

void FighterComponent::Update() {


	if (m_atk < 0) {
		m_atk = 0; // 攻撃力が0以下になったら0にする
	}

	// 一度リセット、ヒットフラッシュ等で使用する
	m_IsAttacked = false;

	if (m_useInvincible == true) {

		if (m_recordTime < 1.5f) {
			if (m_totalDamage > 0 && m_invincibleFlag == false) {
				m_invincibleFlag = true; // ダメージを受けたら無敵フラグを立てる
				m_hp -= m_totalDamage;
				m_totalDamage = 0;
				m_IsAttacked = true;
			}
		}
		else {
			m_recordTime = 0.0f;
			m_invincibleFlag = false;
		}

		// 無敵の経過時間を加算
		if(m_invincibleFlag == true){
			m_recordTime += m_deltaTime;
		}
	}
	else {

		if (m_totalDamage > 0) {
			// 合計ダメージを引く
			m_hp -= m_totalDamage;
			m_totalDamage = 0;
			m_IsAttacked = true;
		}
	}

	if (m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする
		m_deadFlag = true; // 死亡フラグを立てる

		// これがボスであった場合はどうするかを考えてみる
		if (m_useDeadFlag == false) {
			return; // 死亡フラグがfalseなら何もしない
		}
		m_Object->SetDeleteFg(true); // オブジェクトを削除フラグを立てる
	}

}