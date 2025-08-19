#include "TestExtrusionComponent.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include <iostream>

TestExtrusionComponent::TestExtrusionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_EXTRUSION; // ソート番号を設定
}

// 更新処理
void TestExtrusionComponent::Update() {
	auto jump = p_object->GetComponent<JumpComponent>();

	auto extrusionJudge = p_object->GetComponent<TestExtrusionJudgeComponent>();

	//if (coll->CheckHit_AABBAndOBB_IsTrigger3D(collObj2->GetColliderSize_AABB(), coll->GetColliderSize_OBB())) {
	//	std::cout << "AABBとOBBが衝突しました" << std::endl;
	//}
	//else {
	//	std::cout << "AABBとOBBは衝突していません" << std::endl;
	//}

	jump->SetIsGround(extrusionJudge->GetIsGround()); // 地面にいるかどうかを設定
	jump->SetIsCeiling(extrusionJudge->GetIsCeiling()); // 天井にいるかどうかを設定
}