#include "EnemyActionHopperComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render2D.h"
#include "RigidBodyComponent.h"
#include "GameObjectManager.h"
#include "Render3DColliderAABBComponent.h"
#include "JumpComponent.h"
#include "AttackOneTimeComponent.h"
#include "FighterComponent.h"
#include "PlayerDamageComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "SquareMesh.h"

EnemyActionHopperComponent::EnemyActionHopperComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ƒ\[ƒg”Ô†‚ðÝ’è
	auto jump = p_object->AddComponent<JumpComponent>();
	jump->SetJumpPower(60.0f);
}

void EnemyActionHopperComponent::Update() {

	auto player = GameObjectManager::GameObjectFindTag("Player");
	auto playPos = player[0]->GetComponent<TransformComponent>()->GetPosition();
	auto myPos = p_object->GetComponent<TransformComponent>()->GetPosition();
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
		m_rightLeft = false;
	}
	else {
		m_rightLeft = true;
	}

	if (m_recordTime < 2.0f) {
		jumpFlag = true;
	}
	else if (m_recordTime > 2.0f) {
		m_recordTime = 0.0f;
		jumpFlag = false;
	}

	HopperAction(jumpFlag);
}

void EnemyActionHopperComponent::HopperAction(const bool jumpFlag) {
	auto jump = p_object->GetComponent<JumpComponent>();
	auto rigid = p_object->GetComponent<RigidBodyComponent>();
	auto testExtrusion = p_object->GetComponent<TestExtrusionJudgeComponent>();

	bool isGround = testExtrusion->GetIsGround();

	if (isGround == true) {
		rigid->ReduceVelocity_X(0.5f);
	}
	else {
		if (m_rightLeft == false) {
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