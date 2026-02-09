#include "EnemyActionBulletComponent.h"
#include "Transform.h"
#include "ColliderAttackComponent.h"
#include "Render2D.h"
#include "Render3D.h"
#include "RigidBodyComponent.h"
#include "Manager/GameObjectManager.h"
#include "Render3DColliderAABBComponent.h"
#include "BulletComponent.h"
#include "AttackOneTimeComponent.h"
#include "PlayerDamageComponent.h"
#include "Mesh/SquareMesh.h"

#include <iostream>

using namespace DirectX;

namespace {
	constexpr XMFLOAT3 BULLET_SCALE{ 5.0f,5.0f,1.0f };
	constexpr XMFLOAT3 LEFT_VECTOR{ -1.0f, 0.0f, 0.0f };
	constexpr XMFLOAT3 RIGHT_VECTOR{ 1.0f, 0.0f, 0.0f };
	constexpr float FIRING_SPEED = 100.0f;
	constexpr float RIMIT_TIME = 1.0f;
	constexpr float ATTACK_TIME = 3.0f;
	constexpr int BULLET_ATK = 2;
	constexpr int BULLET_HP = 2;
}

EnemyActionBulletComponent::EnemyActionBulletComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void EnemyActionBulletComponent::Update() {

	m_recordTime += m_deltaTime;

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

	const XMFLOAT3 myPos = myTrans->GetPosition();
	const XMFLOAT3 playPos = playTrans->GetPosition();

	if (myPos.x > playPos.x) {
		m_IsRightLeft = RightLeft::LEFT;
	}
	else {
		m_IsRightLeft = RightLeft::RIGHT;
	}

	rend->SetInversionFlag(m_IsRightLeft);

	if (m_recordTime > ATTACK_TIME) {

		FiringBullet();
		m_recordTime = 0.0f;
	}
}

// ’e‚ðì¬
void EnemyActionBulletComponent::FiringBullet() {

	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	if (myTrans == nullptr) {
		return;
	}
	const XMFLOAT3 myPos = myTrans->GetPosition();

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
	rend->SetShader("ShaderResource/Animation2DVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	rend->ChangeTexture("assets/texture/bullet.png");
	rend->SetInversionFlag(m_IsRightLeft);
}