#include "MoveTerrainComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Manager/GameObjectManager.h"
#include "PlayerOperationComponent.h"
#include "RigidBodyComponent.h"
#include <cmath>
#include <iostream>
#include "Manager/TimeManager.h"

using namespace DirectX;

MoveTerrainComponent::MoveTerrainComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定、仮にMOVE_TERRAINを設置
}

void MoveTerrainComponent::Update() {

	m_RecordTime += TimeManager::GetFixedDeltaTime();

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	ColliderComponent* collider = m_Object->GetComponent<ColliderComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	if(transform == nullptr || collider == nullptr || rigid == nullptr) {
		return;
	}

	const XMFLOAT3 myPos = transform->GetPosition();

	const XMFLOAT3 newPos = m_MoveVector * sinf(m_RecordTime) * m_MoveSpeed;

	rigid->ConstantVelocity(newPos);

	if (m_player == nullptr) {
		GameObject* player = GameObjectManager::GameObjectFindName("Player");
		if (player != nullptr) {
			m_player = player;
		}
	}
	else {
		TransformComponent* playerTransform = m_player->GetComponent<TransformComponent>();
		ColliderComponent* collplay = m_player->GetComponent<ColliderComponent>();
		PlayerOperationComponent* testMove = m_player->GetComponent<PlayerOperationComponent>();

		if (playerTransform == nullptr || collplay == nullptr) {
			return;
		}
		XMFLOAT3 hitNormal = {};
		// 押し戻すことを考える
		if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collplay, *collider, hitNormal) == true) {
			if (playerTransform->GetPosition().y > myPos.y) {	// プレイヤーが地面に乗っているとき

				const float delta = myPos.x - m_BeforePos.x;
				if (testMove->GetMoveFlag() == false) {
					playerTransform->AddPosition(XMFLOAT3(delta, 0.0f, 0.0f));
				}
			}

		}
	}

	std::vector<GameObject*> enemies = GameObjectManager::GameObjectFindTag("Enemy");

	for (GameObject* obj : enemies) {

		TransformComponent* enemyTransform = obj->GetComponent<TransformComponent>();
		ColliderComponent* collEnemy = obj->GetComponent<ColliderComponent>();

		if (enemyTransform == nullptr || collEnemy == nullptr) {
			continue;
		}

		XMFLOAT3 hitNormal = {};
		if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collEnemy, *collider, hitNormal) == true) {
			if (enemyTransform->GetPosition().y > myPos.y) {	// 敵が地面に乗っているとき

				if (m_BeforePos != XMFLOAT3())
				{
					const float delta = myPos.x - m_BeforePos.x;

					enemyTransform->AddPosition({ delta,0.0f,0.0f });
				}
			}
		}
	}

	m_BeforePos = myPos;
}