#include "TestExtrusionComponent.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "iostream"

TestExtrusionComponent::TestExtrusionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_EXTRUSION; // ソート番号を設定
}

// 更新処理
void TestExtrusionComponent::Update() {
	auto coll = p_object->GetComponent<ColliderComponent>();
	auto rigid = p_object->GetComponent<RigidBodyComponent>();
	auto jump = p_object->GetComponent<JumpComponent>();

	auto terrains = GameObjectManager::GameObjectFindTag("Terrain");

	isGround = false;

	//if (coll->CheckHit_AABBAndOBB_IsTrigger3D(collObj2->GetColliderSize_AABB(), coll->GetColliderSize_OBB())) {
	//	std::cout << "AABBとOBBが衝突しました" << std::endl;
	//}
	//else {
	//	std::cout << "AABBとOBBは衝突していません" << std::endl;
	//}

	for(auto& terrain : terrains) {
		auto terrainColl = terrain->GetComponent<ColliderComponent>();
		if (terrainColl == nullptr) {
			continue; // コライダーが存在しない場合はスキップ
		}

		DirectX::SimpleMath::Vector3 hitNormal = {};
		if (coll->CheckHit_CubeAndCube_NoTrigger2D_Normal(*terrainColl, *coll,hitNormal)) {
			if (hitNormal.y < -0.5f) {	// 地面
				rigid->UseGravity(false);
				isGround = true;
			}
			else if (hitNormal.y > 0.5f) {	// 天井
				std::cout << "天井" << std::endl;
			}
			else if (abs(hitNormal.x) > 0.5f) { // 左右の壁
				std::cout << "左右" << std::endl;
			}
		}
	}

	jump->SetIsGround(isGround); // 地面にいるかどうかを設定
}