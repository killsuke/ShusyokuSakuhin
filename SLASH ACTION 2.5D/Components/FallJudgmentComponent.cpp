#include "FallJudgmentComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "FighterComponent.h"
#include "TimeLineComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"

using namespace DirectX;

namespace {
	constexpr int DAMAGE = 10; // 落下ダメージ
}

FallJudgmentComponent::FallJudgmentComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

void FallJudgmentComponent::Update() {

	ColliderComponent* collider = m_Object->GetComponent<ColliderComponent>();
	TimeLineComponent* timeLine = m_Object->GetComponent<TimeLineComponent>();

	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	if (player == nullptr || collider == nullptr || timeLine == nullptr) {
		return;
	}

	TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
	ColliderComponent* collplay = player->GetComponent<ColliderComponent>();

	if(playerTransform == nullptr || collplay == nullptr) {
		return;
	}

	XMFLOAT3 hitNormal = {};
	if (collider->CheckHit_CubeAndCube_IsTrigger2D_Normal(*collplay, *collider, hitNormal) == true) {

		// ここでタイムラインを使用する
		timeLine->AddRangeDelayEvent(0.0f, 1.0f, 0.0f, this, nullptr, [this]() {ResurrentionProcess(); }, [this]() {PlayerActiveProcess(); });
	
		//	timeLine->AddPointDelayEvent(0.21f, this, [this]() {CreateCracksAndDebris(); });	// 破片やヒビを生成
	}
}

void FallJudgmentComponent::ResurrentionProcess() {


	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	GameObject* sword = GameObjectManager::GameObjectFindName("sword");

	if (player == nullptr || sword == nullptr) {
		return;
	}

	TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
	ColliderComponent* collplay = player->GetComponent<ColliderComponent>();

	if (playerTransform == nullptr || collplay == nullptr) {
		return;
	}

	playerTransform->SetPosition(m_Resurrection);
	collplay->Update(); // コライダーの位置を更新

	FighterComponent* fighter = player->GetComponent<FighterComponent>();

	if (fighter == nullptr) {
		return;
	}

	const uint32_t myID = m_Object->GetInstanceID();
	const uint32_t otherID = player->GetInstanceID();

	const FallHitEvent he = { myID,otherID,DAMAGE };

	// ヒット時の通知テスト
	EventBusManager::Push(he);

	player->SetActiveState(ActiveState::ALL_STOP);
	sword->SetActiveState(ActiveState::DRAW_STOP);
}

void FallJudgmentComponent::PlayerActiveProcess() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	GameObject* sword = GameObjectManager::GameObjectFindName("sword");

	if (player == nullptr || sword == nullptr) {
		return;
	}

	player->SetActiveState(ActiveState::ACTIVE);
	sword->SetActiveState(ActiveState::ACTIVE);
}