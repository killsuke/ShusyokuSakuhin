#include "TestExtrusionJudgeComponent.h"
#include "Collider.h"
#include "RigidBodyComponent.h"
#include <SimpleMath.h>
#include "GameObjectManager.h"
#include "JumpComponent.h"

using namespace DirectX::SimpleMath;

TestExtrusionJudgeComponent::TestExtrusionJudgeComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_JUDGE_EXTRUSION"); // ソート番号を設定
}

void TestExtrusionJudgeComponent::Update()
{
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();
	auto terrains = gameObjectManager.GameObjectFindTag("Terrain");
	auto coll = p_object->GetComponent<ColliderComponent>();
	auto rigid = p_object->GetComponent<RigidBodyComponent>();
	auto jump = p_object->GetComponent<JumpComponent>();


	m_isGround = false;
	m_isCeiling = false;

	for (auto& terrain : terrains) {
		auto terrainColl = terrain->GetComponent<ColliderComponent>();
		if (terrainColl == nullptr) {
			continue; // コライダーが存在しない場合はスキップ
		}

		Vector3 hitNormal = {};
		if (coll->TestNormal(*terrainColl, *coll, hitNormal)) {
			if (hitNormal.y < -0.5f) {	// 地面
				rigid->UseGravity(false);
				m_isGround = true;
			}
			else if (hitNormal.y > 0.5f) {	// 天井
				m_isCeiling = true;
				//		std::cout << "天井" << std::endl;
			}
			else if (abs(hitNormal.x) > 0.5f) { // 左右の壁
				//		std::cout << "左右" << std::endl;
			}
		}
	}

	if(jump == nullptr) {
		return; // ジャンプコンポーネントが存在しない場合は終了
	}
	jump->SetIsGround(m_isGround); // 地面にいるかどうかを設定
	jump->SetIsCeiling(m_isCeiling); // 天井にいるかどうかを設定
}