#include "BoneComponent.h"

BoneComponent::BoneComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("BONE"); // ƒ\[ƒg”Ô†‚ğİ’è
}

void BoneComponent::Update() {

}