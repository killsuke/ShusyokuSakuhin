#include "EnemyActionBossComponent.h"
#include "Manager/GameObjectManager.h"
#include "Transform.h"
#include "Render2D.h"
#include "FighterComponent.h"
#include "RigidBodyComponent.h"
#include "BulletComponent.h"
#include "AttackOneTimeComponent.h"
#include "PlayerDamageComponent.h"
#include "Collider.h"
#include "Mesh/SquareMesh.h"

EnemyActionBossComponent::EnemyActionBossComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void EnemyActionBossComponent::Update() {
	m_recordTime += m_deltaTime;
	auto player = GameObjectManager::GameObjectFindTag("Player");
	auto playPos = player[0]->GetComponent<TransformComponent>()->GetPosition();
	auto myPos = m_Object->GetComponent<TransformComponent>()->GetPosition();
	auto rend = m_Object->GetComponent<Render2DComponent>();

	if (myPos.x > playPos.x) {
		m_IsRightLeft = RightLeft::LEFT;
	}
	else {
		m_IsRightLeft = RightLeft::RIGHT;
	}
	rend->SetInversionFlag(m_IsRightLeft);
	if (m_recordTime > 1.0f) {
		BossAction();
		m_recordTime = 0.0f;
	}
}

void EnemyActionBossComponent::BossAction() {
	auto myTrans = m_Object->GetComponent<TransformComponent>();
	auto myPos = myTrans->GetPosition();

	// ‚±‚±‚ª’eì¬ˆ—
	auto bullet = GameObjectManager::AddObject("bullet", "Enemy");
	auto trans = bullet->AddComponent<TransformComponent>();
	trans->SetPosition({ myPos.x + 10.0f,myPos.y,myPos.z });
	trans->SetScale({ 10.0f,10.0f,1.0f });
	auto rigid = bullet->AddComponent<RigidBodyComponent>();
	auto bull = bullet->AddComponent<BulletComponent>();
	if (m_IsRightLeft == RightLeft::LEFT) {
		bull->SetFiringVector({ -1.0f, 0.0f, 0.0f });
	}
	else if(m_IsRightLeft == RightLeft::RIGHT){
		bull->SetFiringVector({ 1.0f, 0.0f, 0.0f });
	}
	bull->SetFiringSpeed(100.0f);
	bull->SetRimitTime(1.0f);
	auto fight = bullet->AddComponent<FighterComponent>();
	fight->SetAtk(10);
	fight->SetHp(10);
	auto atk = bullet->AddComponent<AttackOneTimeComponent>();
	auto dmg = bullet->AddComponent<PlayerDamageComponent>();

	auto coll = bullet->AddComponent<ColliderComponent>();
	auto rend = bullet->AddComponent<Render2DComponent>();

	rend->CreateMesh<SquareMesh>();
	rend->SetShader("shader/Animation2DVS.hlsl", "shader/unlitTexturePS.hlsl");
	rend->ChangeTexture("assets/texture/bullet.png");
	rend->SetInversionFlag(m_IsRightLeft);

}