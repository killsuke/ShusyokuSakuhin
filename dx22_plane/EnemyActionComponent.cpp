#include "EnemyActionComponent.h"

EnemyActionComponent::EnemyActionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ƒ\[ƒg”Ô†‚ğİ’è
}

void EnemyActionComponent::Update() {

}
