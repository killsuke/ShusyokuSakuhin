#include "BulletComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include <iostream>

using namespace DirectX;

BulletComponent::BulletComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ソート番号を設定、仮にGO_AROUNDを設置
}

void BulletComponent::Update() {
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	if(rigid == nullptr) {
		return;
	}

	XMFLOAT3 newVelocity = m_firingVector * m_firingSpeed;

	rigid->ConstantVelocity(newVelocity);

	m_rimitTime -= m_deltaTime;

	if (m_rimitTime <= 0.0f) {
		m_Object->SetDeleteFg(true);
	}
}
