#include "BulletComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "Manager/TimeManager.h"
#include <iostream>

using namespace DirectX;

BulletComponent::BulletComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ソート番号を設定、仮にGO_AROUNDを設置
}

void BulletComponent::Update() {

	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();

	if(rigid == nullptr || trans == nullptr) {
		return;
	}

	const XMFLOAT3 newVelocity = m_FiringVector * m_FiringSpeed;

	rigid->ConstantVelocity(newVelocity);

	if (m_IsRotateFlag == true) {
		trans->AddRotation(m_RotateValue);
	}

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	m_RimitTime -= deltaTime;

	if (m_RimitTime <= 0.0f) {

		if (m_IsDestroyOrStop == true) {
			m_Object->Destroy();
		}
		else {
			m_Object->SetActiveState(ActiveState::ALL_STOP);
		}
	}
}
