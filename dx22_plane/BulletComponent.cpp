#include "BulletComponent.h"
#include "RigidBodyComponent.h"
#include "Transform.h"
#include <iostream>

BulletComponent::BulletComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ソート番号を設定、仮にGO_AROUNDを設置
}

void BulletComponent::Update() {
	auto rigid = p_object->GetComponent<RigidBodyComponent>();

	auto newVelocity = m_firingVector * m_firingSpeed;

	rigid->ConstantVelocity(newVelocity);

	m_rimitTime -= m_deltaTime;

	if (m_rimitTime <= 0.0f) {
		p_object->SetDeleteFg(true);
	}
}
