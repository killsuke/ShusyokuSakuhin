#include "AttackComponent.h"

AttackComponent::AttackComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("ATTACK"); // ƒ\[ƒg”Ô†‚ğİ’è
}

void AttackComponent::Update() {



}