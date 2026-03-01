#include "ChargeSlashComponent.h"

namespace {
	constexpr float DeltaTime = 0.016f;
}

ChargeSlashComponent::ChargeSlashComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void ChargeSlashComponent::Update() {

	if (m_RecordTime > m_RimitTime) {
		m_Object->Destroy();
	}

	m_RecordTime += DeltaTime;
}