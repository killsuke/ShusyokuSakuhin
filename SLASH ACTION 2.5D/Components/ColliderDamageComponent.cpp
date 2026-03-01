#include "ColliderDamageComponent.h"

ColliderDamageComponent::ColliderDamageComponent(GameObject& obj) : ColliderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("COLLIDER_DAMAGE"); // ƒ\[ƒg”Ô†‚ğİ’è

}

void ColliderDamageComponent::Update() {

	ColliderComponent::Update();
}