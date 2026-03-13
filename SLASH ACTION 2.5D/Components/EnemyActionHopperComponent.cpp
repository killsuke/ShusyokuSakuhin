#include "EnemyActionHopperComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Render2DComponent.h"
#include "RigidBodyComponent.h"
#include "Manager/GameObjectManager.h"
#include "Render3DColliderAABBComponent.h"
#include "JumpComponent.h"
#include "AttackOneTimeComponent.h"
#include "FighterComponent.h"
#include "PlayerDamageComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "Mesh/SquareMesh.h"
#include "Manager/EventBusManager.h"

using namespace DirectX;

EnemyActionHopperComponent::EnemyActionHopperComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ソート番号を設定
	JumpComponent* jump = m_Object->AddComponent<JumpComponent>();
	jump->SetJumpPower(60.0f);

	m_listenerID_HitEvent_Hopper = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		KnockBackEvent(e);
		});
}

EnemyActionHopperComponent::~EnemyActionHopperComponent() {
	EventBusManager::Unsubscribe(m_listenerID_HitEvent_Hopper);
}

void EnemyActionHopperComponent::Update() {

	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	GameObject* player = GameObjectManager::GameObjectFindName("Player");

	if (player == nullptr || rigid == nullptr) {
		return;
	}

	TransformComponent* playTrans = player->GetComponent<TransformComponent>();
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();

	if (playTrans == nullptr || myTrans == nullptr || rend == nullptr) {
		return;
	}

	const XMFLOAT3 playPos = playTrans->GetPosition();
	const XMFLOAT3 myPos = myTrans->GetPosition();
	bool jumpFlag = false;

	const XMFLOAT3 delta = playPos - myPos;

	const XMVECTOR deltaVec = XMLoadFloat3(&delta);
	const float length = XMVectorGetX(XMVector3Length(deltaVec));

	if (length > 100.0f) {
		m_moveFlag = false;
	}
	else {
		m_moveFlag = true;
	}

	m_recordTime += m_deltaTime;

	if (m_recordTime < 2.0f) {
		jumpFlag = true;
	}
	else if (m_recordTime > 2.0f) {
		m_recordTime = 0.0f;
		jumpFlag = false;
	}

	// ここの向き変更はジャンプ開始のみ
	if (m_IsBeforeJump == false && jumpFlag == true) {
		if (myPos.x > playPos.x) {
			m_IsRightLeft = RightLeft::LEFT;
		}
		else {
			m_IsRightLeft = RightLeft::RIGHT;
		}
	}

	rend->SetInversionFlag(m_IsRightLeft);

	HopperAction(jumpFlag);

	m_IsBeforeJump = jumpFlag;

	//rigid->ReduceVelocity_X(0.5f);
}

// ジャンプの処理
void EnemyActionHopperComponent::HopperAction(const bool jumpFlag) {

	JumpComponent* jump = m_Object->GetComponent<JumpComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	TestExtrusionJudgeComponent* testExtrusion = m_Object->GetComponent<TestExtrusionJudgeComponent>();
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

	if (jump == nullptr || rigid == nullptr || testExtrusion == nullptr || myTrans == nullptr) {
		return;
	}

	bool isGround = testExtrusion->GetIsGround();

	if (isGround == true) {
		rigid->ReduceVelocity_X(0.5f);
	}
	else {
		if (m_IsRightLeft == RightLeft::LEFT) {
			//rigid->ConstantVelocity_X(-30.0f);

			myTrans->AddPosition({ -0.5f,0.0f,0.0f });
		}
		else if (m_IsRightLeft == RightLeft::RIGHT) {
			//rigid->ConstantVelocity_X(30.0f);

			myTrans->AddPosition({ 0.5f,0.0f,0.0f });
		}
	}

	if (m_moveFlag == true) {
		jump->SetJumpPress(jumpFlag);
	}
}

// ノックバックの処理
void EnemyActionHopperComponent::KnockBackEvent(const HitEvent& event) {

	const uint32_t deadID = m_Object->GetInstanceID();

	if (event.targetID != deadID) {
		return; // 自分宛じゃないなら無視
	}

	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	TransformComponent* playerTrans = player->GetComponent<TransformComponent>();
	TransformComponent* enemyTrans = m_Object->GetComponent<TransformComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	if (playerTrans == nullptr || enemyTrans == nullptr || rigid == nullptr) {
		return;
	}

	const XMFLOAT3 playPos = playerTrans->GetPosition();
	const XMFLOAT3 enemyPos = enemyTrans->GetPosition();

	if (enemyPos.x > playPos.x) {
		rigid->AddVelocity_X(100.0f);
	}
	else if (enemyPos.x < playPos.x) {
		rigid->AddVelocity_X(-100.0f);
	}

	rigid->AddVelocity_Y(50.0f);
}