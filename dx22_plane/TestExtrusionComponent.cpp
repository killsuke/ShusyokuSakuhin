#include "TestExtrusionComponent.h"
#include "Collider.h"
#include "GameObjectManager.h"

TestExtrusionComponent::TestExtrusionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_EXTRUSION; // ソート番号を設定
}

// 更新処理
void TestExtrusionComponent::Update() {
	auto coll = p_object->GetComponent<ColliderComponent>();

	auto obj = GameObjectManager::GameObjectFindName("cube2");
	auto collObj = obj->GetComponent<ColliderComponent>();

	DirectX::XMFLOAT3 hitNormal = {};
	if (coll != nullptr && collObj != nullptr) {
		if (coll->CheckHit_CubeAndCube_NoTrigger2D_Normal(collObj->GetColliderSize_AABB(), coll->GetColliderSize_AABB(), hitNormal)) {

		}
	}
}