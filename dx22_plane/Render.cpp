#include "Render.h"
#include "Transform.h"

RenderComponent::RenderComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ƒ\[ƒg”Ô†‚ğİ’è
}
