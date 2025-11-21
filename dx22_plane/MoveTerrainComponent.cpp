#include "MoveTerrainComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include "PlayerOperationComponent.h"
#include "RigidBodyComponent.h"
#include <cmath>
#include <SimpleMath.h>
#include <iostream>

using namespace DirectX::SimpleMath;

MoveTerrainComponent::MoveTerrainComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定、仮にMOVE_TERRAINを設置
}

void MoveTerrainComponent::Update() {

	m_recordTime += m_deltaTime;

	auto transform = m_Object->GetComponent<TransformComponent>();
	auto collider = m_Object->GetComponent<ColliderComponent>();

	auto rigid = m_Object->GetComponent<RigidBodyComponent>();

	float newPosX = sinf(m_recordTime) * m_moveSpeed;

	rigid->ConstantVelocity_X(newPosX);

	if (m_player == nullptr) {
		auto player = GameObjectManager::GameObjectFindTag("Player");
		if (player.size() > 0) {
			m_player = player[0];
		}
	}
	else {
		auto playerTransform = m_player->GetComponent<TransformComponent>();
		auto collplay = m_player->GetComponent<ColliderComponent>();
		PlayerOperationComponent* testMove = m_player->GetComponent<PlayerOperationComponent>();

		if(playerTransform == nullptr || collplay == nullptr) {
			return;
		}
		Vector3 hitNormal = {};
		// 押し戻すことを考える
		if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collplay, *collider, hitNormal) == true) {
			if (playerTransform->GetPosition().y > transform->GetPosition().y) {	// プレイヤーが地面に乗っているとき

				if (testMove->GetMoveFlag() == false) {
					auto rigidP = m_player->GetComponent<RigidBodyComponent>();
					rigidP->AddVelocity({ newPosX * 0.5f ,0,0});
				}
			}
			
		}
	}

	auto enemies = GameObjectManager::GameObjectFindTag("Enemy");

	for(const auto& obj : enemies) {
		auto enemyTransform = obj->GetComponent<TransformComponent>();
		auto collEnemy = obj->GetComponent<ColliderComponent>();
		Vector3 hitNormal = {};
		if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collEnemy, *collider, hitNormal) == true) {
			if (hitNormal.y > 0.5f) {	// 敵が地面に乗っているとき
				enemyTransform->AddPosition({ newPosX,0.0f,0.0f });
			}
		}
	}
}