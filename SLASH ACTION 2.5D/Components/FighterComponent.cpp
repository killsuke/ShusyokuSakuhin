#include "FighterComponent.h"
#include "Manager/GameObjectManager.h"
#include <iostream>
#include <string>
#include <DirectXMath.h>
#include "EnemyActionComponent.h"
#include "TransformComponent.h"
#include "Manager/EventBusManager.h"
#include "Manager/TimeManager.h"

namespace {
	constexpr float INVINCIBLE_TIME = 1.5f; // 無敵時間
}

FighterComponent::FighterComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("FIGHTER"); // ソート番号を設定
	m_ListenerID_HitEvent = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		OnHit(e);
		});

	m_ListenerID_FallHitEvent = EventBusManager::Subscribe<FallHitEvent>([&](const FallHitEvent& e) {
		OnFallHit(e);
		});
}

FighterComponent::~FighterComponent() {

	EventBusManager::Unsubscribe(m_ListenerID_HitEvent);
	EventBusManager::Unsubscribe(m_ListenerID_FallHitEvent);
}

void FighterComponent::Update() {

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	if (m_Atk < 0) {
		m_Atk = 0; // 攻撃力が0以下になったら0にする
	}

	// 無敵の経過時間を加算
	if (m_InvincibleFlag == true) {
		m_RecordTime += deltaTime;

		// 無敵時間終了判定
		if (m_RecordTime >= INVINCIBLE_TIME) {
			m_RecordTime = 0.0f;
			m_InvincibleFlag = false;
		}
	}
}

// 敵から受けるダメージ
void FighterComponent::DamageProcess(const HitEvent& event) {

	if (m_UseInvincible == true) {

		if (m_RecordTime < INVINCIBLE_TIME) {
			if (m_TotalDamage > 0 && m_InvincibleFlag == false) {
				m_InvincibleFlag = true; // ダメージを受けたら無敵フラグを立てる
				m_Hp -= m_TotalDamage;

				DamageEvent de = {
					event.attackerID,
					event.targetID,
					m_TotalDamage
				};

				EventBusManager::Push(de);

				m_TotalDamage = 0;
			}
		}
		else {
			m_RecordTime = 0.0f;
			m_InvincibleFlag = false;
		}
	}
	else {

		if (m_TotalDamage > 0) {
			// 合計ダメージを引く
			m_Hp -= m_TotalDamage;

			DamageEvent de = {
					event.attackerID,
					event.targetID,
					m_TotalDamage
			};

			EventBusManager::Push(de);
			m_TotalDamage = 0;
		}
	}

	if (m_Hp <= 0) {
		m_Hp = 0;

		if (m_UseDeadFlag == false) {

			if (m_DeadFlag == false) {
				m_DeadFlag = true; // 死亡フラグを立てる
				const uint32_t id = m_Object->GetInstanceID();
				const DeathEvent de = { id };

				EventBusManager::Push(de);
			}
			return;
		}

		m_DeadFlag = true; // 死亡フラグを立てる
	}
}

// 落下した場合のダメージ
void FighterComponent::FallDamageProcess(const FallHitEvent& event) {

	if (m_UseInvincible == true) {

		if (event.damage > 0) {
			m_InvincibleFlag = true; // ダメージを受けたら無敵フラグを立てる
			m_Hp -= event.damage;

			FallDamageEvent de = {
				event.attackerID,
				event.targetID,
				event.damage
			};

			EventBusManager::Push(de);

			m_RecordTime = 0.0f; // 無敵時間の記録をリセット
		}
	}
	else {

		if (event.damage > 0) {
			// 合計ダメージを引く
			m_Hp -= event.damage;

			FallDamageEvent de = {
					event.attackerID,
					event.targetID,
					event.damage
			};

			EventBusManager::Push(de);
		}
	}

	if (m_Hp <= 0) {
		m_Hp = 0;

		if (m_UseDeadFlag == false) {

			if (m_DeadFlag == false) {
				m_DeadFlag = true; // 死亡フラグを立てる
				const uint32_t id = m_Object->GetInstanceID();
				const DeathEvent de = { id };

				EventBusManager::Push(de);
			}
			return;
		}

		m_DeadFlag = true; // 死亡フラグを立てる
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