#include "EnemyDamageComponent.h"
#include "Collider.h"
#include "Transform.h"
#include "AttackTimingComponent.h"
#include "AttackOneTimeComponent.h"
#include "GameObjectManager.h"

EnemyDamageComponent::EnemyDamageComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("DAMAGE"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void EnemyDamageComponent::Update()
{
	//auto transform = p_object->GetComponent<TransformComponent>();
	auto collObjMe = p_object->GetComponent<ColliderComponent>();
	auto objOthers = GameObjectManager::GameObjectFindTag("Enemy");

	//	auto collObjOther = objOther->GetComponent<ColliderComponent>();
	//	auto playerObj = GameObjectManager::GameObjectFindName("Player");
	//	auto playerTrans = playerObj->GetComponent<TransformComponent>();
	//	auto playerPos = playerTrans->GetPosition();
		//transform->SetPosition({playerPos.x + 13.0f,playerPos.y,playerPos.z});

	//auto attack = p_object->GetComponent<AttackTimingComponent>();
	auto attack = p_object->GetComponent<AttackOneTimeComponent>();

	if (collObjMe != nullptr) {

		for (auto& objOther : objOthers) {
			auto collObjOther = objOther->GetComponent<ColliderComponent>();
			if (collObjMe->CheckHit_AABBAndOBB_IsTrigger3D(
				*collObjOther, *collObjMe)) {

				attack->AttackAction(*objOther);
			}

		}
	}
}