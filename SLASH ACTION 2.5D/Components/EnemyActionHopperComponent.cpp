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

namespace {

	constexpr float DETECTION_RANGE = 100.0f;
	constexpr float ATTACK_TIMING = 0.3f;
	constexpr float JUMP_TIMING = 1.0f;
	constexpr float DAMAGE_POSE_FIN_TIME = 1.0f;
	constexpr float GROUND_BETWEEN_TIME = 1.5f;

	constexpr XMFLOAT2 DEFAULT_POSE{ 1.0f,1.0f };
	constexpr XMFLOAT2 POSTURE_POSE{ 2.0f,1.0f };
	constexpr XMFLOAT2 ATTACK_POSE{ 3.0f,1.0f };
	constexpr XMFLOAT2 DAMAGE_POSE{ 4.0f,1.0f };
	constexpr XMFLOAT2 ANIM_CUT{ 4.0f,1.0f };
}

EnemyActionHopperComponent::EnemyActionHopperComponent(GameObject& obj) :EnemyActionComponent(obj) {

	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ソート番号を設定
	JumpComponent* jump = nullptr;

	if (jump == nullptr) {
		jump = m_Object->AddComponent<JumpComponent>();
		jump->SetJumpPower(60.0f);
	}

	m_ListenerID_HitEvent_Hopper = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		KnockBackEvent(e);
		});
}

EnemyActionHopperComponent::~EnemyActionHopperComponent() {
	EventBusManager::Unsubscribe(m_ListenerID_HitEvent_Hopper);
}

void EnemyActionHopperComponent::Update() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");

	if (player == nullptr) {
		return;
	}

	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	TestExtrusionJudgeComponent* testExtrusion = m_Object->GetComponent<TestExtrusionJudgeComponent>();

	if (rend == nullptr || rigid == nullptr || testExtrusion == nullptr) {
		return;
	}

	bool isGround = testExtrusion->GetIsGround();

	if (isGround == true) {
		rigid->ReduceVelocity_X(0.5f);
	}

	m_RecordTime += TimeManager::GetFixedDeltaTime();

	rend->SetInversionFlag(m_IsRightLeft);

	StateUpdate(player);
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

	if (m_IsRightLeft == RightLeft::LEFT) {
		//rigid->ConstantVelocity_X(-30.0f);

		myTrans->AddPosition({ -0.5f,0.0f,0.0f });
	}
	else if (m_IsRightLeft == RightLeft::RIGHT) {
		//rigid->ConstantVelocity_X(30.0f);

		myTrans->AddPosition({ 0.5f,0.0f,0.0f });
	}

	jump->SetJumpPress(jumpFlag);
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

	m_RecordTime = 0.0f; // ジャンプの時間をリセット
	m_IsFear = true; // アクティブにする
	m_RecordFearTime = 0.0f; // 怯み時間の記録もリセット
	m_FearPower = FEAR_POWER;	// 怯みの強さをセット

	ChangeState(EEnemyState::DAMAGED);
}

// ステートチェンジ
void EnemyActionHopperComponent::ChangeState(const EEnemyState& newState) {

	if (m_EnemyState == newState) {
		return; // 状態が変わっていないなら何もしない
	}

	switch (m_EnemyState) {

	case EEnemyState::WAIT:
		break;
	case EEnemyState::MOVE:
		break;
	case EEnemyState::ATTACK:
	{
		JumpComponent* jump = m_Object->GetComponent<JumpComponent>();

		if (jump != nullptr) {
			jump->SetJumpPress(false);
		}

		m_RecordTime = 0.0f;
	}

	break;
	case EEnemyState::DAMAGED:
		break;
	default:
		break;
	}

	m_EnemyState = newState;

	switch (m_EnemyState) {

	case EEnemyState::WAIT:
		break;
	case EEnemyState::MOVE:
		break;
	case EEnemyState::ATTACK:

		ChangeDirection();
		break;
	case EEnemyState::DAMAGED:
		break;
	default:
		break;
	}
}

// ステートの更新
void EnemyActionHopperComponent::StateUpdate(GameObject* player) {

	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();
	TestExtrusionJudgeComponent* testExtrusion = m_Object->GetComponent<TestExtrusionJudgeComponent>();
	TransformComponent* playTrans = player->GetComponent<TransformComponent>();
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

	if (playTrans == nullptr || myTrans == nullptr || testExtrusion == nullptr || rend == nullptr) {
		return;
	}

	Mesh* mesh = rend->GetMesh();

	if (mesh == nullptr) {
		return;
	}

	mesh->SetInitialCut(ANIM_CUT);
	mesh->SetCutNum(DEFAULT_POSE);

	bool isGround = testExtrusion->GetIsGround();

	const XMFLOAT3 playPos = playTrans->GetPosition();
	const XMFLOAT3 myPos = myTrans->GetPosition();

	const XMFLOAT3 delta = playPos - myPos;

	const XMVECTOR deltaVec = XMLoadFloat3(&delta);
	const float length = XMVectorGetX(XMVector3Length(deltaVec));

	switch (m_EnemyState) {

	case EEnemyState::WAIT:

		if (length < DETECTION_RANGE) {

			if (m_RecordTime > ATTACK_TIMING) {

				ChangeState(EEnemyState::ATTACK);
			}
		}
		else {

			m_RecordTime = 0.0f;
		}

		break;
	case EEnemyState::MOVE:
		break;
	case EEnemyState::ATTACK:

		// ジャンプ
		if (m_RecordTime > JUMP_TIMING) {

			HopperAction(true);
		}
		// 構え
		else if (m_RecordTime > ATTACK_TIMING) {

			mesh->SetCutNum(POSTURE_POSE);
		}

		// 着地
		if (isGround == true) {

			if (m_RecordTime > GROUND_BETWEEN_TIME) {
				ChangeState(EEnemyState::WAIT);
			}
		}
		// ジャンプ中
		else {

			mesh->SetCutNum(ATTACK_POSE);
		}

		break;
	case EEnemyState::DAMAGED:

		mesh->SetCutNum(DAMAGE_POSE);
		if (m_RecordTime > DAMAGE_POSE_FIN_TIME && isGround == true) {
			ChangeState(EEnemyState::WAIT);
		}

		break;
	default:
		break;
	}

}

// 右と左切り替え
void EnemyActionHopperComponent::ChangeDirection() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");

	if (player == nullptr) {
		return;
	}

	TransformComponent* playTrans = player->GetComponent<TransformComponent>();
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

	if (playTrans == nullptr || myTrans == nullptr) {
		return;
	}

	const XMFLOAT3 myPos = myTrans->GetPosition();
	const XMFLOAT3 playPos = playTrans->GetPosition();

	// ここの向き変更はジャンプ開始のみ
	if (myPos.x > playPos.x) {
		m_IsRightLeft = RightLeft::LEFT;
	}
	else {
		m_IsRightLeft = RightLeft::RIGHT;
	}
}
