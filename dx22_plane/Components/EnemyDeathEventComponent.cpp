#include "EnemyDeathEventComponent.h"
#include "MeshCut2DComponent.h"
#include "VectorMoveComponent.h"
#include "RigidBodyComponent.h"
#include "ProjectileMotionComponent.h"
#include "Manager/EventBusManager.h"
#include "Manager/GameObjectManager.h"
#include <SimpleMath.h>

using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
}

EnemyDeathEventComponent::EnemyDeathEventComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("DOOR_FADE"); // ソート番号を設定

	m_listenerID_DeathEvent = EventBusManager::Subscribe<DeathEvent>([&](const DeathEvent& e) {
		DeathEventAction(e);
		});

	// 最初は生きているのでフラグをOFFに
	m_IsActiveFlag = false;
}

EnemyDeathEventComponent::~EnemyDeathEventComponent() {
	EventBusManager::Unsubscribe(m_listenerID_DeathEvent);
}

void EnemyDeathEventComponent::Update() {

	MeshCut2DComponent* meshCut = m_Object->GetComponent<MeshCut2DComponent>();
	uint32_t cutObj1ID = meshCut->GetCutObj1ID();
	uint32_t cutObj2ID = meshCut->GetCutObj2ID();
	GameObject* meshCutObj1 = GameObjectManager::GameObjectFindInstanceID(cutObj1ID);
	GameObject* meshCutObj2 = GameObjectManager::GameObjectFindInstanceID(cutObj2ID);

	switch (m_State)
	{
		// 切れてから少しズレる処理
	case EnemyDeathEventState::DELAY:


		break;

		// 即座に切れる処理
	case EnemyDeathEventState::IMMEDIATE:


		break;
	default:
		break;
	}

	if (meshCutObj1 == nullptr || meshCutObj2 == nullptr) {
		return; // 切断オブジェクトが存在しないなら何もしない
	}

	// 一定時間経過したらオブジェクトを消去する
	if (m_RecordTime > 1.7f) {
		meshCut->DeleteCutObjs();
	}
	// 一定時間経過したら重力を有効にして放物運動させる
	else if (m_RecordTime > 0.7f) {
		VectorMoveComponent* move1 = meshCutObj1->GetComponent<VectorMoveComponent>();
		VectorMoveComponent* move2 = meshCutObj2->GetComponent<VectorMoveComponent>();

		move1->SetActiveFlag(false);
		move2->SetActiveFlag(false);

		RigidBodyComponent* rigid1 = meshCutObj1->GetComponent<RigidBodyComponent>();
		RigidBodyComponent* rigid2 = meshCutObj2->GetComponent<RigidBodyComponent>();

		rigid1->SetGravityFlag(true);
		rigid2->SetGravityFlag(true);

		ProjectileMotionComponent* proj1 = meshCutObj1->GetComponent<ProjectileMotionComponent>();
		ProjectileMotionComponent* proj2 = meshCutObj2->GetComponent<ProjectileMotionComponent>();

		proj1->SetProjectilePower(10.0f);
		proj2->SetProjectilePower(10.0f);

		proj1->SetProjectileRotation({ 0.0f,0.0f,15.0f });
		proj2->SetProjectileRotation({ 0.0f,0.0f,-15.0f });
	}
	// 最初の一回だけ動かす
	// 切れてズレる
	else if (m_RecordTime == 0.0f) {
		VectorMoveComponent* move1 = meshCutObj1->GetComponent<VectorMoveComponent>();
		VectorMoveComponent* move2 = meshCutObj2->GetComponent<VectorMoveComponent>();

		move1->SetMoveDirection({ 0.0f,1.0f,0.0f });
		move2->SetMoveDirection({ 0.0f,-1.0f,0.0f });

		move1->SetMovePower(0.1f);
		move2->SetMovePower(0.1f);

		RigidBodyComponent* rigid1 = meshCutObj1->AddComponent<RigidBodyComponent>();
		RigidBodyComponent* rigid2 = meshCutObj2->AddComponent<RigidBodyComponent>();

		rigid1->SetGravityFlag(false);
		rigid2->SetGravityFlag(false);

		rigid1->SetMass(2.0f);
		rigid2->SetMass(2.0f);

		ProjectileMotionComponent* proj1 = meshCutObj1->AddComponent<ProjectileMotionComponent>();
		ProjectileMotionComponent* proj2 = meshCutObj2->AddComponent<ProjectileMotionComponent>();

		proj1->SetProjectileDirection({ -0.3f,1.0f,-1.8f });
		proj2->SetProjectileDirection({ 0.3f,1.0f,-1.8f });
	}

	m_RecordTime += DeltaTime;
}

void EnemyDeathEventComponent::DeathEventAction(const DeathEvent& event) {
	const uint32_t deadID = m_Object->GetInstanceID();

	if (event.deadID != deadID) {
		return; // 自分宛じゃないなら無視
	}

	CutEvent ce;

	CutDirection dir = CutDirection::VERTICAL;
	float ratio1 = 0.5f;
	float ratio2 = 0.5f;

	// 切断方向と割合を決定
	// 斜め横切り
	if (m_SwordActionState == ESwordActionState::SLASH_1ST) {
		dir = CutDirection::HORIZONTAL;
		if(m_RightLeft == RightLeft::RIGHT){
			ratio1 = 0.8f;
			ratio2 = 0.2f;
		}
		else if(m_RightLeft == RightLeft::LEFT){
			ratio1 = 0.2f;
			ratio2 = 0.8f;
		}
	}
	// 斜め横切り
	else if (m_SwordActionState == ESwordActionState::SLASH_2ND) {
		dir = CutDirection::HORIZONTAL;
		if (m_RightLeft == RightLeft::RIGHT) {
			ratio1 = 0.2f;
			ratio2 = 0.8f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
			ratio1 = 0.8f;
			ratio2 = 0.2f;
		}

	}
	// 縦切り
	else if (m_SwordActionState == ESwordActionState::SLASH_3RD || m_SwordActionState == ESwordActionState::NONE) {
		dir = CutDirection::VERTICAL;
		ratio1 = 0.5f;
		ratio2 = 0.5f;
	}

	ce = CutEvent{
		deadID,
		dir,
		ratio1,ratio2
	};

	// ヒット時の通知テスト
	EventBusManager::Push(ce);

	m_IsActiveFlag = true;
}
