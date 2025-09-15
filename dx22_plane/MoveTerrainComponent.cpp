#include "MoveTerrainComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include <cmath>
#include <SimpleMath.h>

MoveTerrainComponent::MoveTerrainComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定、仮にMOVE_TERRAINを設置
}

void MoveTerrainComponent::Update() {

	m_recordTime += m_deltaTime;

	auto transform = p_object->GetComponent<TransformComponent>();
	auto collider = p_object->GetComponent<ColliderComponent>();

	float newPosX = sinf(m_recordTime) * m_moveSpeed;

	transform->AddPosition({ newPosX,0.0f,0.0f });

	if (m_player == nullptr) {
		auto player = GameObjectManager::GameObjectFindTag("Player");
		if (player.size() > 0) {
			m_player = player[0];
		}
	}
	else {
		auto playerTransform = m_player->GetComponent<TransformComponent>();
		auto collplay = m_player->GetComponent<ColliderComponent>();

		if(playerTransform == nullptr || collplay == nullptr) {
			return;
		}
		DirectX::SimpleMath::Vector3 hitNormal = {};
		if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collplay, *collider, hitNormal) == true) {
			if (hitNormal.y > 0.5f) {	// プレイヤーが地面に乗っているとき
				playerTransform->AddPosition({ newPosX,0.0f,0.0f });
			}
		}
	}

	auto enemies = GameObjectManager::GameObjectFindTag("Enemy");

	for(const auto& obj : enemies) {
		auto enemyTransform = obj->GetComponent<TransformComponent>();
		auto collEnemy = obj->GetComponent<ColliderComponent>();
		DirectX::SimpleMath::Vector3 hitNormal = {};
		if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collEnemy, *collider, hitNormal) == true) {
			if (hitNormal.y > 0.5f) {	// 敵が地面に乗っているとき
				enemyTransform->AddPosition({ newPosX,0.0f,0.0f });
			}
		}
	}

}