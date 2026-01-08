#include "LuminescenceBillboardControlComponent.h"

LuminescenceBillboardControlComponent::LuminescenceBillboardControlComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_CONTOROL"); // ソート番号を仮置き
}

void LuminescenceBillboardControlComponent::Update() {




}