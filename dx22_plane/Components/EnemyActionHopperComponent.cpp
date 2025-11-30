#include "EnemyActionHopperComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render2D.h"
#include "RigidBodyComponent.h"
#include "Manager/GameObjectManager.h"
#include "Render3DColliderAABBComponent.h"
#include "JumpComponent.h"
#include "AttackOneTimeComponent.h"
#include "FighterComponent.h"
#include "PlayerDamageComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "Mesh/SquareMesh.h"

EnemyActionHopperComponent::EnemyActionHopperComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ƒ\[ƒg”Ô†‚ðÝ’è
	auto jump = m_Object->AddComponent<JumpComponent>();
	jump->SetJumpPower(60.0f);
}

void EnemyActionHopperComponent::Update() {

	auto player = GameObjectManager::GameObjectFindTag("Player");
	auto playPos = player[0]->GetComponent<TransformComponent>()->GetPosition();
	auto myPos = m_Object->GetComponent<TransformComponent>()->GetPosition();
	auto rend = m_Object->GetComponent<Render2DComponent>();
	bool jumpFlag = false;

	float length = (playPos - myPos).Length();

	if (length > 100.0f) {
		m_moveFlag = false;
	}
	else {
		m_moveFlag = true;
	}

	m_recordTime += m_deltaTime;

	if (myPos.x > playPos.x) {
		m_IsRightLeft = false;
	}
	else {
		m_IsRightLeft = true;
	}

	if (m_recordTime < 2.0f) {
		jumpFlag = true;
	}
	else if (m_recordTime > 2.0f) {
		m_recordTime = 0.0f;
		jumpFlag = false;
	}

	rend->SetInversionFlag(!m_IsRightLeft);

	HopperAction(jumpFlag);
}

void EnemyActionHopperComponent::HopperAction(const bool jumpFlag) {
	auto jump = m_Object->GetComponent<JumpComponent>();
	auto rigid = m_Object->GetComponent<RigidBodyComponent>();
	auto testExtrusion = m_Object->GetComponent<TestExtrusionJudgeComponent>();
	bool isGround = testExtrusion->GetIsGround();

	if (isGround == true) {
		rigid->ReduceVelocity_X(0.5f);
	}
	else {
		if (m_IsRightLeft == false) {
			rigid->ConstantVelocity_X(-30.0f);
		}
		else {
			rigid->ConstantVelocity_X(30.0f);
		}
	}

	if (m_moveFlag == true) {
		jump->SetJumpPress(jumpFlag);
	}
}