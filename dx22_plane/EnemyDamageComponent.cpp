#include "EnemyDamageComponent.h"
#include "Collider.h"
#include "Transform.h"
#include "AttackComponent.h"
#include "GameObjectManager.h"

EnemyDamageComponent::EnemyDamageComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ENEMY_DAMAGE; // ソート番号を設定
}

void EnemyDamageComponent::Update()
{
	//auto transform = p_object->GetComponent<TransformComponent>();
	auto collObjMe = p_object->GetComponent<ColliderComponent>();
	auto objOther = GameObjectManager::GameObjectFindName("Enemy");
	if(objOther == nullptr) {
		return; // "Enemy"オブジェクトが見つからない場合は何もしない
	}
	auto collObjOther = objOther->GetComponent<ColliderComponent>();
	auto playerObj = GameObjectManager::GameObjectFindName("Player");
	auto playerTrans = playerObj->GetComponent<TransformComponent>();
	auto playerPos = playerTrans->GetPosition();
	//transform->SetPosition({playerPos.x + 13.0f,playerPos.y,playerPos.z});

	auto attack = p_object->GetComponent<AttackComponent>();

	if (collObjMe != nullptr && collObjOther != nullptr) {

		if (collObjMe->CheckHit_CubeAndCube_IsTrigger3D(
			collObjMe->GetColliderSize_AABB(), collObjOther->GetColliderSize_AABB())) {

			attack->Attack(*objOther);
		}
	}

}