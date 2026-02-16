#include "StretchingComponent.h"
#include "RenderCharacterComponent.h"
#include "Manager/TimeManager.h"
#include <DirectXMath.h>

using namespace DirectX;

StretchingComponent::StretchingComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_CONTOROL"); // ソート番号を設定
}

void StretchingComponent::Update()
{
	RenderCharacterComponent* renderCharacter = m_Object->GetComponent<RenderCharacterComponent>();

	if(renderCharacter != nullptr) {

		const float deltaTime = TimeManager::GetFixedDeltaTime();
		m_RecordTime += deltaTime;

		float raw = std::sinf(2.0f * XM_PI * m_StretchSpeed * m_RecordTime);	// -1.0f ～ 1.0f の値が周期的に変化する

		if(raw < -0.7f) {
			 
			m_HolderTime += deltaTime;
			if (m_HolderTime < 0.2f) {
				raw = -0.7f;	// 伸縮の振幅を最大にする
			}
			else {
				m_HolderTime = 0.0f;	// 保持時間をリセット
			}
		}
		
		m_CurrentScale = m_StretchPower * raw;

		renderCharacter->SetBaseScale(m_BaseScale);
		renderCharacter->SetCurrentScale(m_CurrentScale);
	}
}