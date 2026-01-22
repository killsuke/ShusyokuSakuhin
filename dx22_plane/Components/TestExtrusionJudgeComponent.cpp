#include "TestExtrusionJudgeComponent.h"
#include "Collider.h"
#include "RigidBodyComponent.h"
#include <SimpleMath.h>
#include "Manager/GameObjectManager.h"
#include "JumpComponent.h"
#include "Transform.h"
#include "PlayerOperationComponent.h"
#include <iostream>

using namespace DirectX::SimpleMath;

TestExtrusionJudgeComponent::TestExtrusionJudgeComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_JUDGE_EXTRUSION"); // ソート番号を設定
}

void TestExtrusionJudgeComponent::Update()
{
	auto terrains = GameObjectManager::GameObjectFindTag("Terrain");
	auto coll = m_Object->GetComponent<ColliderComponent>();
	auto rigid = m_Object->GetComponent<RigidBodyComponent>();
	auto jump = m_Object->GetComponent<JumpComponent>();
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	m_isGround = false;
	m_isCeiling = false;

	for (auto& terrain : terrains) {
		ColliderComponent* terrainColl = terrain->GetComponent<ColliderComponent>();
		if (terrainColl == nullptr) {
			continue; // コライダーが存在しない場合はスキップ
		}

		Vector3 hitNormal = {};
		if (coll->TestNormal(*terrainColl, *coll, hitNormal)) {
			if (hitNormal.y < -0.5f) {	// 地面
				rigid->UseGravity(false);
				m_isGround = true;

				MoveCarComponent* moveCarComp = terrain->GetComponent<MoveCarComponent>();
				std::string terrainName = terrain->GetName();

				if (moveCarComp != nullptr && terrainName != terrainName) {
					m_MoveCarObjName = terrainName;
				}
			}
			else if (hitNormal.y > 0.5f) {	// 天井
				m_isCeiling = true;
				//		std::cout << "天井" << std::endl;
			}
			else if (abs(hitNormal.x) > 0.5f) { // 左右の壁

				PlayerOperationComponent* testMove = m_Object->GetComponent<PlayerOperationComponent>();
				if (testMove != nullptr) {
					//RightLeft rightLeft = testMove->GetRightLeft();
					//bool isJump = testMove->GetIsJump();
					//TransformComponent* terrainTrans = terrain->GetComponent<TransformComponent>();
					//// ここに壁けりの処理を入れる
					//Vector3 terrainPos = terrainTrans->GetPosition();
					//Vector3 myPos = transform->GetPosition();

					// この実際の壁けりの処理は分ける（地面と壁を分けるため）
					/*if (isJump == true) {
						if (myPos.x < terrainPos.x) {
							rigid->AddVelocity(Vector3(-400.0f, 100.0f, 0.0f));
							rigid->SetLimitVelocity_X(400.0f);
							rigid->SetLimitVelocity_Y(100.0f);
						}
						else {
							rigid->AddVelocity(Vector3(400.0f, 100.0f, 0.0f));
							rigid->SetLimitVelocity_X(400.0f);
							rigid->SetLimitVelocity_Y(100.0f);
						}

						rigid->UpdateVelocity();
					}*/
					//		std::cout << "左右" << std::endl;
				}
			}
		}

	}

	if (m_MoveCarObjName != "") {

		GameObject* moveCarObj = GameObjectManager::GameObjectFindName(m_MoveCarObjName);

		if (moveCarObj != nullptr) {

			MoveCarComponent* moveCar = moveCarObj->GetComponent<MoveCarComponent>();

			// 名前検索の処理も入れる

			//float speed = moveCar->GetMoveSpeed();
			//// ここの2倍は外そう
			//rigid->AddForce(Vector3(speed * 2.0f, 0.0f, 0.0f));
			//rigid->UpdateVelocity();

			Vector3 deltaPos = moveCar->GetDeltaCarPos();

			transform->AddPosition(deltaPos * 2.0f);
		}
	}
	coll->Update();

	if (jump == nullptr) {
		return; // ジャンプコンポーネントが存在しない場合は終了
	}
	jump->SetIsGround(m_isGround); // 地面にいるかどうかを設定
	jump->SetIsCeiling(m_isCeiling); // 天井にいるかどうかを設定
}