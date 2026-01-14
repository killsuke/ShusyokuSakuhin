#include "EnemyDamageComponent.h"
#include "Collider.h"
#include "Transform.h"
#include "AttackTimingComponent.h"
#include "AttackOneTimeComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include "Manager/HitStopManager.h"
#include "FighterComponent.h"
#include "EnemyDeathEventComponent.h"

namespace {
	constexpr float FirstStopTime = 0.1f; // 最初のヒットストップ時間
	constexpr float SecondStopTime = 0.2f; // ２回目のヒットストップ時間
	constexpr float ThirdStopTime = 0.3f; // ３回目のヒットストップ時間

	const ShakeStatus FirstShake = ShakeStatus(50.0f,1.5f,0.2f);
	const ShakeStatus SecondShake = ShakeStatus(55.0f,1.8f,0.2f);
	const ShakeStatus ThirdShake = ShakeStatus(70.0f,2.0f,0.3f);
}

EnemyDamageComponent::EnemyDamageComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("DAMAGE"); // ソート番号を設定
}

void EnemyDamageComponent::Update()
{
	auto collObjMe = m_Object->GetComponent<ColliderComponent>();
	auto objOthers = GameObjectManager::GameObjectFindTag("Enemy");

	//	auto collObjOther = objOther->GetComponent<ColliderComponent>();
	//	auto playerObj = GameObjectManager::GameObjectFindName("Player");
	//	auto playerTrans = playerObj->GetComponent<TransformComponent>();
	//	auto playerPos = playerTrans->GetPosition();
		//transform->SetPosition({playerPos.x + 13.0f,playerPos.y,playerPos.z});

	//auto attack = p_object->GetComponent<AttackTimingComponent>();
	AttackOneTimeComponent* attack = m_Object->GetComponent<AttackOneTimeComponent>();

	attack->ReSetAttackHitFlag();	// 攻撃が当たったかどうかのフラグをリセット

	if (collObjMe != nullptr) {

		for (auto& objOther : objOthers) {
			ColliderComponent* collObjOther = objOther->GetComponent<ColliderComponent>();
			if (collObjMe->CheckHit_AABBAndOBB_IsTrigger3D(
				*collObjOther, *collObjMe)) {

				attack->AttackAction(*objOther);

				if (attack->GetAttackHitFlag() == true) {

					std::string tag = m_Object->GetTag();

					TestSwordActionComponent* swordComp = m_Object->GetComponent<TestSwordActionComponent>();

					// チャージ攻撃のことも考えて取り敢えずデフォルト
					ESwordActionState state = ESwordActionState::NONE;
					RightLeft dir = RightLeft::RIGHT;

					// ヒットストップ時間の選択
					float stopTime = FirstStopTime;
					if (swordComp != nullptr) {
						state = swordComp->GetSwordActionState();
						stopTime = ChoiceStopTime(state);
						dir = swordComp->GetRightLeft();
					}

					// 敵の切られた状態をセット
					EnemyDeathEventComponent* deathComp = objOther->GetComponent<EnemyDeathEventComponent>();
					if (deathComp != nullptr) {
						deathComp->SetHittedState(state, dir);
					}

					// ヒットストップの候補セット
					HitStopManager::AddTargetTags({ "Player","Enemy","Sword","Effect","SkyDome"});
					HitStopManager::SetHitStopTime(stopTime); // ヒットストップ時間をセット

					GameObject* camera = GameObjectManager::GameObjectFindName("camera");
					if (camera != nullptr) {
						CameraShakeComponent* shake = camera->GetComponent<CameraShakeComponent>();

						const uint32_t myID = m_Object->GetInstanceID();
						const uint32_t otherID = objOther->GetInstanceID();

						const HitEvent he = { myID,otherID };

						ShakeStatus status = FirstShake;

						// ヒット時の通知
						EventBusManager::Push(he);

						// 画面揺れステータスの選択
						if (swordComp != nullptr) {
							ESwordActionState state = swordComp->GetSwordActionState();
							status = ChoiceShakeStatus(state);
						}

						// 画面揺れ開始
						shake->ShakingPreparation(status);
					}
				}
			}
		}
	}
}

float EnemyDamageComponent::ChoiceStopTime(const ESwordActionState& state) {

	switch (state)
	{
	case ESwordActionState::SLASH_1ST:
		return FirstStopTime;
		break;

	case ESwordActionState::SLASH_2ND:
		return SecondStopTime;
		break;

	case ESwordActionState::SLASH_3RD:
		return ThirdStopTime;
		break;
	default:
		return FirstStopTime;
		break;
	}
}

ShakeStatus EnemyDamageComponent::ChoiceShakeStatus(const ESwordActionState& state) {

	switch (state)
	{
	case ESwordActionState::SLASH_1ST:
		return FirstShake;
		break;

	case ESwordActionState::SLASH_2ND:
		return SecondShake;
		break;

	case ESwordActionState::SLASH_3RD:
		return ThirdShake;
		break;
	default:
		return FirstShake;
		break;
	}
}
