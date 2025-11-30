#include "EnemyActionBulletComponent.h"
#include "Transform.h"
#include "Collider.h"
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

EnemyActionBulletComponent::EnemyActionBulletComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ソート番号を設定
}

void EnemyActionBulletComponent::Update() {

	m_recordTime += m_deltaTime;

	auto player = GameObjectManager::GameObjectFindTag("Player");
	auto playPos = player[0]->GetComponent<TransformComponent>()->GetPosition();
	auto myPos = m_Object->GetComponent<TransformComponent>()->GetPosition();
	auto rend = m_Object->GetComponent<Render2DComponent>();

	if (myPos.x > playPos.x) {
		m_IsRightLeft = false;
	}
	else {
		m_IsRightLeft = true;
	}

	rend->SetInversionFlag(!m_IsRightLeft);

	if (m_recordTime > 3.0f) {

		FiringBullet();
		m_recordTime = 0.0f;
	}
}

void EnemyActionBulletComponent::FiringBullet() {

	auto myTrans = m_Object->GetComponent<TransformComponent>();
	auto myPos = myTrans->GetPosition();

	auto bullet = GameObjectManager::AddObject("bullet", "Bullets");
	auto trans = bullet->AddComponent<TransformComponent>();
	trans->SetPosition({ myPos.x,myPos.y,myPos.z });
	trans->SetScale({ 5.0f,5.0f,1.0f });
	auto rigid = bullet->AddComponent<RigidBodyComponent>();
	auto bull = bullet->AddComponent<BulletComponent>();
	if (m_IsRightLeft == false) {
		bull->SetFiringVector({ -1.0f, 0.0f, 0.0f });
	}
	else {
		bull->SetFiringVector({ 1.0f, 0.0f, 0.0f });
	}
	bull->SetFiringSpeed(100.0f);
	bull->SetRimitTime(1.0f);
	auto fight = bullet->AddComponent<FighterComponent>();
	fight->SetAtk(2);
	fight->SetHp(2);
	auto atk = bullet->AddComponent<AttackOneTimeComponent>();
	auto dmg = bullet->AddComponent<PlayerDamageComponent>();

	auto coll = bullet->AddComponent<ColliderComponent>();

	// 毎回シェーダー生成をしているから重い。
	auto rend = bullet->AddComponent<Render2DComponent>();

	rend->CreateMesh<SquareMesh>();
	rend->SetShader("shader/Animation2DVS.cso", "shader/unlitTexturePS.cso");
	rend->ChangeTexture("assets/texture/bullet.png");
	rend->SetInversionFlag(!m_IsRightLeft);
}