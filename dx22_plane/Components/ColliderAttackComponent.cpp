#include "ColliderAttackComponent.h"

ColliderAttackComponent::ColliderAttackComponent(GameObject& obj) : ColliderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("COLLIDER_ATTACK"); // ƒ\[ƒg”Ô†‚ğİ’è

}

void ColliderAttackComponent::Update() {

	ColliderComponent::Update();
}