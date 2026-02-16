#include "StretchingComponent.h"
#include "RenderCharacterComponent.h"

StretchingComponent::StretchingComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_CONTOROL"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void StretchingComponent::Update()
{
	RenderCharacterComponent* renderCharacter = m_Object->GetComponent<RenderCharacterComponent>();

	if(renderCharacter != nullptr) {
		renderCharacter->SetBaseScale(m_BaseScale);
		renderCharacter->SetCurrentScale(m_CurrentScale);
	}
}