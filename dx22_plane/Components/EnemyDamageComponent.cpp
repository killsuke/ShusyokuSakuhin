#include "EnemyDamageComponent.h"
#include "Collider.h"
#include "Transform.h"
#include "AttackTimingComponent.h"
#include "AttackOneTimeComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include "HitStopManager.h"
#include "CameraShakeComponent.h"
#include "FighterComponent.h"

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
	auto attack = m_Object->GetComponent<AttackOneTimeComponent>();

	attack->ReSetAttackHitFlag();	// 攻撃が当たったかどうかのフラグをリセット

	if (collObjMe != nullptr) {

		for (auto& objOther : objOthers) {
			auto collObjOther = objOther->GetComponent<ColliderComponent>();
			if (collObjMe->CheckHit_AABBAndOBB_IsTrigger3D(
				*collObjOther, *collObjMe)) {

				attack->AttackAction(*objOther);

				if (attack->GetAttackHitFlag() == true) {
					std::vector<std::string> targetTags = { "Player","Enemy","Sword","Effect","SkyDome" };
					for (const auto& tag : targetTags)
					{
						HitStopManager::AddTargetTag(tag); // ヒットストップ対象タグを追加
					}
					HitStopManager::SetHitStopTime(0.1f); // ヒットストップ時間をセット
					GameObject* camera = GameObjectManager::GameObjectFindName("camera");
					if (camera != nullptr) {
						CameraShakeComponent* shake = camera->GetComponent<CameraShakeComponent>();
						
						HitEvent he = { m_Object,objOther };

						// ヒット時の通知
						EventBusManager::Push(he);

						// 画面揺れ開始
						shake->ShakingPreparation(50.0f,1.5f,0.2f);
					}
				}
			}

		}
	}
}