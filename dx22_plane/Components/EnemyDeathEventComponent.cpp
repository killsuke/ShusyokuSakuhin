#include "EnemyDeathEventComponent.h"
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
	if (meshCut == nullptr) {
		return; // メッシュカットコンポーネントがないなら何もしない
	}

	uint32_t cutObj1ID = meshCut->GetCutObj1ID();
	uint32_t cutObj2ID = meshCut->GetCutObj2ID();
	GameObject* meshCutObj1 = GameObjectManager::GameObjectFindInstanceID(cutObj1ID);
	GameObject* meshCutObj2 = GameObjectManager::GameObjectFindInstanceID(cutObj2ID);

	if (meshCutObj1 == nullptr || meshCutObj2 == nullptr) {
		return; // 切断オブジェクトが存在しないなら何もしない
	}

	switch (m_State)
	{
		// 切れてから少しズレる処理
	case EnemyDeathEventState::IMMEDIATE:

		ImmediateProcess(meshCut, meshCutObj1, meshCutObj2);
		break;

		// 吹っ飛んでから斬れる処理
	case EnemyDeathEventState::STICKY:

		StickyProcess(meshCut, meshCutObj1, meshCutObj2);
		break;
	default:
		break;
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
		if (m_RightLeft == RightLeft::RIGHT) {
			ratio1 = 0.8f;
			ratio2 = 0.2f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
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

void EnemyDeathEventComponent::ImmediateProcess(MeshCut2DComponent* cutComp, GameObject* obj1, GameObject* obj2) {


	// 一定時間経過したらオブジェクトを消去する
	if (m_RecordTime > 1.7f) {
		cutComp->DeleteCutObjs();
	}
	// 最初の一回だけ動かす
	// 切れてズレる
	else if (m_RecordTime == 0.0f) {	// 切れたら即飛ばす！

		RigidBodyComponent* rigid1 = obj1->AddComponent<RigidBodyComponent>();
		RigidBodyComponent* rigid2 = obj2->AddComponent<RigidBodyComponent>();

		rigid1->SetGravityFlag(true);
		rigid2->SetGravityFlag(true);

		rigid1->SetMass(2.0f);
		rigid2->SetMass(2.0f);

		ProjectileMotionComponent* proj1 = obj1->AddComponent<ProjectileMotionComponent>();
		ProjectileMotionComponent* proj2 = obj2->AddComponent<ProjectileMotionComponent>();

		proj1->SetProjectileDirection({ -0.3f,1.0f,-1.8f });
		proj2->SetProjectileDirection({ 0.3f,1.0f,-1.8f });

		proj1->SetProjectilePower(28.0f);
		proj2->SetProjectilePower(28.0f);

		proj1->SetProjectileRotation({ 0.0f,0.0f,15.0f });
		proj2->SetProjectileRotation({ 0.0f,0.0f,-15.0f });
	}
}

void EnemyDeathEventComponent::StickyProcess(MeshCut2DComponent* cutComp, GameObject* obj1, GameObject* obj2) {

	// 画面に張り付いたら一瞬だけ大きくしてすぐに収縮
	// 収縮したら揺らしてからバラけて落ちる

	if (m_RecordTime > 3.0f) {
	}
	else if (m_RecordTime == 0.0f) {

		VectorMoveComponent* move1 = obj1->GetComponent<VectorMoveComponent>();
		move1->SetMoveDirection({ 0.0f,1.0f,0.0f });
		move1->SetMovePower(7.0f);

		VectorMoveComponent* move2 = obj2->GetComponent<VectorMoveComponent>();
		move2->SetMoveDirection({ 0.0f,1.0f,0.0f });
		move2->SetMovePower(7.0f);


	}
}