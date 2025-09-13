#include "EnemyActionBulletComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render2D.h"
#include "Render3D.h"
#include "RigidBodyComponent.h"
#include "GameObjectManager.h"
#include "Render3DColliderAABBComponent.h"
#include "BulletComponent.h"
#include "AttackOneTimeComponent.h"
#include "FighterComponent.h"
#include "PlayerDamageComponent.h"
#include "SquareMesh.h"

#include <iostream>

EnemyActionBulletComponent::EnemyActionBulletComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void EnemyActionBulletComponent::Update() {

	m_recordTime += m_deltaTime;

	auto player = GameObjectManager::GameObjectFindTag("Player");
	auto playPos = player[0]->GetComponent<TransformComponent>()->GetPosition();
	auto myPos = p_object->GetComponent<TransformComponent>()->GetPosition();

	if (myPos.x > playPos.x) {
		m_rightLeft = false;
	}
	else {
		m_rightLeft = true;
	}

	if (m_recordTime > 5.0f) {
		FiringBullet();
		m_recordTime = 0.0f;
	}
}

void EnemyActionBulletComponent::FiringBullet() {
	
	auto myTrans = p_object->GetComponent<TransformComponent>();
	auto myPos = myTrans->GetPosition();

	auto bullet = GameObjectManager::AddObject("bullet", "Bullets");
	auto trans = bullet->AddComponent<TransformComponent>();
	trans->SetPosition({ myPos.x,myPos.y,myPos.z });
	trans->SetScale({ 5.0f,5.0f,1.0f });
	auto rigid = bullet->AddComponent<RigidBodyComponent>();
	auto bull = bullet->AddComponent<BulletComponent>();
	if (m_rightLeft == false) {
		bull->SetFiringVector({ -1.0f, 0.0f, 0.0f });
	}
	else {
		bull->SetFiringVector({ 1.0f, 0.0f, 0.0f });
	}
	bull->SetFiringSpeed(10.0f);
	auto fight = bullet->AddComponent<FighterComponent>();
	fight->SetAtk(2);
	fight->SetHp(2);
	auto atk = bullet->AddComponent<AttackOneTimeComponent>();
	auto dmg = bullet->AddComponent<PlayerDamageComponent>();

	auto coll = bullet->AddComponent<ColliderComponent>();
	auto rend = bullet->AddComponent<Render3DComponent>();
	SquareMesh squareMesh;
	rend->SetMesh(squareMesh);
	rend->SetShader("Animation2DVS.hlsl", "shader/unlitTexturePS.hlsl");
	rend->SetTexture("assets/texture/goal.png");
//	bullet->SetActiveState(ActiveState::UPDATE_STOP);

}