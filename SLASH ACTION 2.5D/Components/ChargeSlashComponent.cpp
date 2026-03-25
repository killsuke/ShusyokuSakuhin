#include "ChargeSlashComponent.h"
#include "Manager/TimeManager.h"

ChargeSlashComponent::ChargeSlashComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void ChargeSlashComponent::Update() {

	if (m_RecordTime > m_RimitTime) {
		m_Object->Destroy();
	}

	m_RecordTime += TimeManager::GetFixedDeltaTime();
}