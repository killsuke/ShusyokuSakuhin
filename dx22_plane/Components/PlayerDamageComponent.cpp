#include "PlayerDamageComponent.h"
#include "Collider.h"
#include "Transform.h"
#include "RigidBodyComponent.h"
#include "AttackTimingComponent.h"
#include "AttackOneTimeComponent.h"
#include "GameObjectManager.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

PlayerDamageComponent::PlayerDamageComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("DAMAGE"); // ソート番号を設定
}

void PlayerDamageComponent::Update()
{
	//auto transform = p_object->GetComponent<TransformComponent>();
	auto collObjMe = m_Object->GetComponent<ColliderComponent>();
	auto objOthers = GameObjectManager::GameObjectFindTag("Player");

	//	auto collObjOther = objOther->GetComponent<ColliderComponent>();
	//	auto playerObj = GameObjectManager::GameObjectFindName("Player");
	//	auto playerTrans = playerObj->GetComponent<TransformComponent>();
	//	auto playerPos = playerTrans->GetPosition();
		//transform->SetPosition({playerPos.x + 13.0f,playerPos.y,playerPos.z});

	auto attack_T = m_Object->GetComponent<AttackTimingComponent>();
	auto attack_O = m_Object->GetComponent<AttackOneTimeComponent>();

	if (attack_O != nullptr) {
		attack_O->ReSetAttackHitFlag();
	}
	if (attack_T != nullptr) {
		attack_T->ReSetAttackHitFlag();
	}

	// ここでプレイヤーをちょっと吹っ飛ばす処理も追加する予定
	if (collObjMe != nullptr) {

		for (auto& objOther : objOthers) {
			auto collObjOther = objOther->GetComponent<ColliderComponent>();
			if (collObjMe->CheckHit_AABBAndOBB_IsTrigger3D(
				*collObjOther, *collObjMe)) {

				/*RigidBodyComponent* playerRigid = objOther->GetComponent<RigidBodyComponent>();

				if (playerRigid != nullptr) {

					Vector3 playerVelocity = playerRigid->GetVelocity();

					if (playerVelocity.x > 0.0f) {
						playerRigid->AddVelocity(Vector3(80.0f, 0.0f, 0.0f));
					}
					else {
						playerRigid->AddVelocity(Vector3(-80.0f, 0.0f, 0.0f));
					}
				}*/

				if (attack_O != nullptr) {
					attack_O->AttackAction(*objOther);
				}
				if (attack_T != nullptr) {
					attack_T->AttackAction(*objOther);
				}
			}
		}
	}
}