#include "TestExtrusionJudgeComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include <DirectXMath.h>
#include "Manager/GameObjectManager.h"
#include "JumpComponent.h"
#include "TransformComponent.h"
#include "PlayerOperationComponent.h"
#include <iostream>

using namespace DirectX;

TestExtrusionJudgeComponent::TestExtrusionJudgeComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_JUDGE_EXTRUSION"); // ソート番号を設定
}

void TestExtrusionJudgeComponent::Update()
{
	std::vector<GameObject*> terrains = GameObjectManager::GameObjectFindTag("Terrain");
	ColliderComponent* coll = m_Object->GetComponent<ColliderComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	JumpComponent* jump = m_Object->GetComponent<JumpComponent>();
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if(terrains.empty() || coll == nullptr || rigid == nullptr || transform == nullptr) {
		return; // テレインが存在しない、または必要なコンポーネントが存在しない場合は終了
	}

	m_IsGround = false;
	m_IsCeiling = false;

	for (GameObject* terrain : terrains) {

		// 自分自身はスキップ
		if (terrain == m_Object) {
			continue;
		}

		ColliderComponent* terrainColl = terrain->GetComponent<ColliderComponent>();
		if (terrainColl == nullptr) {
			continue; // コライダーが存在しない場合はスキップ
		}

		XMFLOAT3 hitNormal = {};
		if (coll->TestNormal(*terrainColl, *coll, hitNormal)) {
			if (hitNormal.y < -0.5f) {	// 地面
				rigid->UseGravity(false);
				m_IsGround = true;

				MoveCarComponent* moveCarComp = terrain->GetComponent<MoveCarComponent>();
				std::string terrainName = terrain->GetName();

				if (moveCarComp != nullptr && terrainName != terrainName) {
					m_MoveCarObjName = terrainName;
				}
			}
			else if (hitNormal.y > 0.5f) {	// 天井
				m_IsCeiling = true;
				//		std::cout << "天井" << std::endl;
			}
			else if (abs(hitNormal.x) > 0.5f) { // 左右の壁

			}
		}
	}

	coll->Update();

	if (jump == nullptr) {
		return; // ジャンプコンポーネントが存在しない場合は終了
	}
	jump->SetIsGround(m_IsGround); // 地面にいるかどうかを設定
	jump->SetIsCeiling(m_IsCeiling); // 天井にいるかどうかを設定
}