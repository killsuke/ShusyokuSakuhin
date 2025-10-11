#include "PlayerDamageComponent.h"
#include "Collider.h"
#include "Transform.h"
#include "AttackTimingComponent.h"
#include "AttackOneTimeComponent.h"
#include "GameObjectManager.h"

PlayerDamageComponent::PlayerDamageComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("DAMAGE"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void PlayerDamageComponent::Update()
{
	//auto transform = p_object->GetComponent<TransformComponent>();
	auto collObjMe = p_object->GetComponent<ColliderComponent>();
	auto objOthers = GameObjectManager::GameObjectFindTag("Player");

	//	auto collObjOther = objOther->GetComponent<ColliderComponent>();
	//	auto playerObj = GameObjectManager::GameObjectFindName("Player");
	//	auto playerTrans = playerObj->GetComponent<TransformComponent>();
	//	auto playerPos = playerTrans->GetPosition();
		//transform->SetPosition({playerPos.x + 13.0f,playerPos.y,playerPos.z});

	auto attack_T = p_object->GetComponent<AttackTimingComponent>();
	auto attack_O = p_object->GetComponent<AttackOneTimeComponent>();

	if (attack_O != nullptr) {
		attack_O->ReSetAttackHitFlag();
	}
	if (attack_T != nullptr) {
		attack_T->ReSetAttackHitFlag();
	}

	if (collObjMe != nullptr) {

		for (auto& objOther : objOthers) {
			auto collObjOther = objOther->GetComponent<ColliderComponent>();
			if (collObjMe->CheckHit_AABBAndOBB_IsTrigger3D(
				*collObjOther, *collObjMe)) {

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