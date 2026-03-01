#include "DeathBlockComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "FighterComponent.h"
#include "Manager/GameObjectManager.h"

using namespace DirectX;

DeathBlockComponent::DeathBlockComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void DeathBlockComponent::Update() {
	auto collider = m_Object->GetComponent<ColliderComponent>();

	auto player = GameObjectManager::GameObjectFindTag("Player");
	if (player.size() > 0) {
		auto m_player = player[0];
		auto fighter = m_player->GetComponent<FighterComponent>();

		auto playerTransform = m_player->GetComponent<TransformComponent>();
		auto collplay = m_player->GetComponent<ColliderComponent>();

		if (playerTransform == nullptr || collplay == nullptr) {
			return;
		}
		
		XMFLOAT3 hitNormal = {};
		if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collplay, *collider, hitNormal) == true) {
			fighter->SetHp(0);
		}
	}
}