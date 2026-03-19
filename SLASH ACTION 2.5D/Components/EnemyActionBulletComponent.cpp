#include "EnemyActionBulletComponent.h"
#include "TransformComponent.h"
#include "ColliderAttackComponent.h"
#include "Render2DComponent.h"
#include "Render3DComponent.h"
#include "RigidBodyComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include "Render3DColliderAABBComponent.h"
#include "BulletComponent.h"
#include "AttackOneTimeComponent.h"
#include "PlayerDamageComponent.h"

#include <iostream>

using namespace DirectX;

namespace {
	constexpr XMFLOAT3 BULLET_SCALE{ 5.0f,5.0f,1.0f };
	constexpr XMFLOAT3 LEFT_VECTOR{ -1.0f, 0.0f, 0.0f };
	constexpr XMFLOAT3 RIGHT_VECTOR{ 1.0f, 0.0f, 0.0f };
	constexpr XMFLOAT2 DEFAULT_POSE{ 1.0f,1.0f };
	constexpr XMFLOAT2 ATTACK_POSE{ 2.0f,1.0f };
	constexpr XMFLOAT2 DAMAGE_POSE{ 3.0f,1.0f };
	constexpr XMFLOAT2 ANIM_CUT{ 3.0f,1.0f };
	constexpr float FIRING_SPEED = 100.0f;
	constexpr float RIMIT_TIME = 1.0f;
	constexpr float ATTACK_TIME = 3.0f;
	constexpr float PRELIMINARY_OPERATION_TIME = 0.7f;
	constexpr int BULLET_ATK = 2;
	constexpr int BULLET_HP = 2;
}

EnemyActionBulletComponent::EnemyActionBulletComponent(GameObject& obj) :EnemyActionComponent(obj) {

	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ソート番号を設定

	m_listenerID_HitEvent_Bullet = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		FearEvent(e);
		});
}

EnemyActionBulletComponent::~EnemyActionBulletComponent() {
	EventBusManager::Unsubscribe(m_listenerID_HitEvent_Bullet);
}

void EnemyActionBulletComponent::Init() {

}

void EnemyActionBulletComponent::Update() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	if (player == nullptr) {
		return;
	}

	TransformComponent* playTrans = player->GetComponent<TransformComponent>();
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();

	if (playTrans == nullptr || myTrans == nullptr || rend == nullptr) {
		return;
	}

	Mesh* mesh = rend->GetMesh();

	if (mesh == nullptr) {
		return;
	}

	const XMFLOAT3 myPos = myTrans->GetPosition();
	const XMFLOAT3 playPos = playTrans->GetPosition();
	const float deltaTime = TimeManager::GetFixedDeltaTime();
	m_RecordTime += deltaTime;

	ChangeDirection(myPos, playPos);

	rend->SetInversionFlag(m_IsRightLeft);

	//	FearAction();	// 怯み状態の処理

	StateUpdate(deltaTime, myPos, playPos, *mesh);	// 状態に応じた処理
}

// 弾を作成
void EnemyActionBulletComponent::FiringBullet(const DirectX::XMFLOAT3& myPos) {

	GameObject* bullet = GameObjectManager::AddObject("bullet", "Bullets");
	TransformComponent* trans = bullet->AddComponent<TransformComponent>();
	trans->SetPosition({ myPos.x,myPos.y,myPos.z });
	trans->SetScale(BULLET_SCALE);
	RigidBodyComponent* rigid = bullet->AddComponent<RigidBodyComponent>();
	BulletComponent* bull = bullet->AddComponent<BulletComponent>();
	if (m_IsRightLeft == RightLeft::LEFT) {
		bull->SetFiringVector(LEFT_VECTOR);
	}
	else if (m_IsRightLeft == RightLeft::RIGHT) {
		bull->SetFiringVector(RIGHT_VECTOR);
	}
	bull->SetFiringSpeed(FIRING_SPEED);
	bull->SetRimitTime(RIMIT_TIME);
	FighterComponent* fight = bullet->AddComponent<FighterComponent>();
	fight->SetAtk(BULLET_ATK);
	fight->SetHp(BULLET_HP);
	AttackOneTimeComponent* atk = bullet->AddComponent<AttackOneTimeComponent>();
	PlayerDamageComponent* dmg = bullet->AddComponent<PlayerDamageComponent>();

	ColliderComponent* coll = bullet->AddComponent<ColliderComponent>();
	ColliderAttackComponent* attack = bullet->AddComponent<ColliderAttackComponent>();

	Render2DComponent* rend = bullet->AddComponent<Render2DComponent>();

	rend->CreateMesh<SquareMesh>();
	rend->SetShader("Animation2DVS.hlsl", "unlitTexturePS.hlsl");
	rend->ChangeTexture("bullet.png");
	rend->SetInversionFlag(m_IsRightLeft);
}

void EnemyActionBulletComponent::FearEvent(const HitEvent& event) {

	const uint32_t deadID = m_Object->GetInstanceID();

	if (event.targetID != deadID) {
		return; // 自分宛じゃないなら無視
	}

	m_RecordTime = 0.0f; // 攻撃されたら時間をリセット

	m_IsFear = true;
	m_RecordFearTime = 0.0f; // 怯み時間の記録もリセット
	m_FearPower = FEAR_POWER;	// 怯みの強さをセット

	ChangeState(EEnemyState::DAMAGED);
}

void EnemyActionBulletComponent::ChangeState(const EEnemyState& newState) {

	if (m_EnemyState == newState) {
		return; // 状態が変わっていないなら何もしない
	}

	switch (newState) {

	case EEnemyState::WAIT:
		break;
	case EEnemyState::MOVE:
		break;
	case EEnemyState::ATTACK:
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
		break;
	case EEnemyState::DAMAGED:
		break;
	default:
		break;
	}
}

void EnemyActionBulletComponent::StateUpdate(const float deltaTime, const DirectX::XMFLOAT3& myPos, const DirectX::XMFLOAT3& playPos, Mesh& mesh) {

	const float ratio = m_RecordTime / ATTACK_TIME;

	switch (m_EnemyState) {

	case EEnemyState::WAIT:

		mesh.SetInitialCut(ANIM_CUT);
		mesh.SetCutNum(DEFAULT_POSE);

		if (ratio > PRELIMINARY_OPERATION_TIME) {

			ChangeState(EEnemyState::ATTACK);
		}

		break;
	case EEnemyState::MOVE:

		break;
	case EEnemyState::ATTACK:

		if (m_RecordTime > ATTACK_TIME) {

			FiringBullet(myPos);
			m_RecordTime = 0.0f;

			ChangeState(EEnemyState::WAIT);
			break;
		}

		// 攻撃前の予備動作
		if (ratio > PRELIMINARY_OPERATION_TIME) {

			ChangeState(EEnemyState::ATTACK);

			const int newRatio = static_cast<int>(ratio / deltaTime) % 2;

			if (newRatio == 0) {
				mesh.SetCutNum(DEFAULT_POSE);
			}
			else {
				mesh.SetCutNum(ATTACK_POSE);
			}
		}

		break;
	case EEnemyState::DAMAGED:

		mesh.SetCutNum(DAMAGE_POSE);
		FearAction();	// 怯み状態の処理
		break;
	default:
		break;
	}
}

void EnemyActionBulletComponent::ChangeDirection(const DirectX::XMFLOAT3& myPos, const DirectX::XMFLOAT3& playPos) {

	if (myPos.x > playPos.x) {
		m_IsRightLeft = RightLeft::LEFT;
	}
	else {
		m_IsRightLeft = RightLeft::RIGHT;
	}
}

void EnemyActionBulletComponent::DeadAnimation() {

	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();

	if (rend == nullptr) {
		return;
	}

	Mesh* mesh = rend->GetMesh();

	if (mesh == nullptr) {
		return;
	}

	mesh->SetCutNum(DAMAGE_POSE);
}