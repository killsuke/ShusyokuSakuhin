#include "BoneComponent.h"

BoneComponent::BoneComponent(GameObject& obj) :Component(obj) {
	m_sortNum = BONE;
}

void BoneComponent::Update() {

}