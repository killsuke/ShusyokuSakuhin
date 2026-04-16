#include "FallJudgmentComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "FighterComponent.h"
#include "TimeLineComponent.h"
#include "RigidBodyComponent.h"
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

	ColliderComponent* collplay = player->GetComponent<ColliderComponent>();

	if (collplay == nullptr) {
		return;
	}

	if (collider->CheckHit_CubeAndCube_IsTrigger3D(*collplay, *collider) == true) {

		// ここでタイムラインを使用する
		timeLine->AddRangeDelayEvent(0.0f, 1.0f, 0.0f, this, [this](float) {PlayerStop(); }, [this]() {ResurrentionProcess(); }, [this]() {PlayerActiveProcess(); });
	}
}

void FallJudgmentComponent::ResurrentionProcess() {


	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	GameObject* sword = GameObjectManager::GameObjectFindAllName("sword");

	if (player == nullptr || sword == nullptr) {
		return;
	}

	TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
	ColliderComponent* collplay = player->GetComponent<ColliderComponent>();
	ColliderComponent* myCollider = m_Object->GetComponent<ColliderComponent>();
	TimeLineComponent* timeLine = m_Object->GetComponent<TimeLineComponent>();
	RigidBodyComponent* rigid = player->GetComponent<RigidBodyComponent>();

	if (playerTransform == nullptr || collplay == nullptr || myCollider == nullptr || timeLine == nullptr || rigid == nullptr) {
		return;
	}

	FighterComponent* fighter = player->GetComponent<FighterComponent>();

	if (fighter == nullptr) {
		return;
	}

	const int totalDamage = fighter->GetTotalDamage();
	const int hp = fighter->GetHp();

	const int predictionDamage = totalDamage + DAMAGE;

	const int predictedHp = hp - predictionDamage;

	const uint32_t myID = m_Object->GetInstanceID();
	const uint32_t otherID = player->GetInstanceID();

	const FallHitEvent he = { myID,otherID,DAMAGE };

	// ヒット時の通知テスト
	EventBusManager::Push(he);

	if (predictedHp > 0) {

		playerTransform->SetPosition(m_Resurrection);
		collplay->SetActiveColliderFlag(false);
		rigid->ClearVelocity();
	}
	else {

		myCollider->SetActiveColliderFlag(false); // 自身のコライダーを無効にする
	}
}

void FallJudgmentComponent::PlayerStop() {


	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	GameObject* sword = GameObjectManager::GameObjectFindAllName("sword");

	if (player == nullptr || sword == nullptr) {
		return;
	}

	player->SetActiveState(ActiveState::ALL_STOP);
	sword->SetActiveState(ActiveState::DRAW_STOP);
}

void FallJudgmentComponent::PlayerActiveProcess() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	GameObject* sword = GameObjectManager::GameObjectFindAllName("sword");

	if (player == nullptr || sword == nullptr) {
		return;
	}

	ColliderComponent* collplay = player->GetComponent<ColliderComponent>();

	if (collplay == nullptr) {
		return;
	}

	collplay->SetActiveColliderFlag(true); // コライダーを有効にする

	player->SetActiveState(ActiveState::ACTIVE);
	sword->SetActiveState(ActiveState::ACTIVE);
}