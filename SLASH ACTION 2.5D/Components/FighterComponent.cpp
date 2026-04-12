#include "FighterComponent.h"
#include "Manager/GameObjectManager.h"
#include <iostream>
#include <string>
#include <DirectXMath.h>
#include "EnemyActionComponent.h"
#include "TransformComponent.h"
#include "Manager/EventBusManager.h"
#include "Manager/TimeManager.h"

FighterComponent::FighterComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("FIGHTER"); // ソート番号を設定
	m_listenerID_HitEvent = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		OnHit(e);
		});

	m_listenerID_FallHitEvent = EventBusManager::Subscribe<FallHitEvent>([&](const FallHitEvent& e) {
		OnFallHit(e);
		});
}

FighterComponent::~FighterComponent() {
	EventBusManager::Unsubscribe(m_listenerID_HitEvent);
	EventBusManager::Unsubscribe(m_listenerID_FallHitEvent);
}

void FighterComponent::Update() {

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	if (m_atk < 0) {
		m_atk = 0; // 攻撃力が0以下になったら0にする
	}

	// 無敵の経過時間を加算
	if (m_invincibleFlag == true) {
		m_recordTime += deltaTime;

		// 無敵時間終了判定
		if (m_recordTime >= 1.5f) {
			m_recordTime = 0.0f;
			m_invincibleFlag = false;
		}
	}

	if (m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする

		// 遅れて死ぬ
		m_DeadRecordTime += deltaTime;
		if (m_DeadRecordTime > 5.0f) {
			m_Object->SetDeleteFg(true); // オブジェクトを削除フラグを立てる
		}

		// これがボスであった場合はどうするかを考えてみる
		if (m_useDeadFlag == false) {

			if (m_deadFlag == false) {
				m_deadFlag = true; // 死亡フラグを立てる

			}
			return; // 死亡フラグがfalseなら何もしない
		}

		m_deadFlag = true; // 死亡フラグを立てる
	}
}

void FighterComponent::DamageProcess(const HitEvent& event) {

	if (m_useInvincible == true) {

		if (m_recordTime < 1.5f) {
			if (m_totalDamage > 0 && m_invincibleFlag == false) {
				m_invincibleFlag = true; // ダメージを受けたら無敵フラグを立てる
				m_hp -= m_totalDamage;

				DamageEvent de = {
					event.attackerID,
					event.targetID,
					m_totalDamage
				};

				EventBusManager::Push(de);

				m_totalDamage = 0;
			}
		}
		else {
			m_recordTime = 0.0f;
			m_invincibleFlag = false;
		}
	}
	else {

		if (m_totalDamage > 0) {
			// 合計ダメージを引く
			m_hp -= m_totalDamage;

			DamageEvent de = {
					event.attackerID,
					event.targetID,
					m_totalDamage
			};

			EventBusManager::Push(de);
			m_totalDamage = 0;
		}
	}

	if (m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする

		// これがボスであった場合はどうするかを考えてみる
		if (m_useDeadFlag == false) {

			if (m_deadFlag == false) {
				m_deadFlag = true; // 死亡フラグを立てる
				const uint32_t id = m_Object->GetInstanceID();
				const DeathEvent de = { id };

				EventBusManager::Push(de);
			}
			return; // 死亡フラグがfalseなら何もしない
		}

		m_deadFlag = true; // 死亡フラグを立てる
	}
}

void FighterComponent::FallDamageProcess(const FallHitEvent& event) {

	if (m_useInvincible == true) {

		if (event.damage > 0) {
			m_invincibleFlag = true; // ダメージを受けたら無敵フラグを立てる
			m_hp -= event.damage;

			FallDamageEvent de = {
				event.attackerID,
				event.targetID,
				event.damage
			};

			EventBusManager::Push(de);

			m_recordTime = 0.0f; // 無敵時間の記録をリセット
		}
	}
	else {

		if (event.damage > 0) {
			// 合計ダメージを引く
			m_hp -= event.damage;

			FallDamageEvent de = {
					event.attackerID,
					event.targetID,
					event.damage
			};

			EventBusManager::Push(de);
		}
	}

	if (m_hp <= 0) {
		m_hp = 0; // ヒットポイントが0以下になったら0にする

		// これがボスであった場合はどうするかを考えてみる
		if (m_useDeadFlag == false) {

			if (m_deadFlag == false) {
				m_deadFlag = true; // 死亡フラグを立てる
				const uint32_t id = m_Object->GetInstanceID();
				const DeathEvent de = { id };

				EventBusManager::Push(de);
			}
			return; // 死亡フラグがfalseなら何もしない
		}

		m_deadFlag = true; // 死亡フラグを立てる
	}
}

void FighterComponent::OnHit(const HitEvent& event) {

	const uint32_t id = m_Object->GetInstanceID();

	if (event.targetID != id) {
		return; // 自分宛じゃないなら無視
	}
	// ダメージを受ける処理
	DamageProcess(event);
}

void FighterComponent::OnFallHit(const FallHitEvent& event) {

	const uint32_t id = m_Object->GetInstanceID();

	if (event.targetID != id) {
		return; // 自分宛じゃないなら無視
	}

	FallDamageProcess(event);
}