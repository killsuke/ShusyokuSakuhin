#include "BlinkingUIComponent.h"
#include "Render2DComponent.h"
#include "Manager/TimeManager.h"

using namespace DirectX;

BlinkingUIComponent::BlinkingUIComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

void BlinkingUIComponent::Update() {
	
	if (m_IsBlinkingFlag == false) {
		return; // 点滅フラグがfalseなら何もしない
	}

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	m_RecordTime += deltaTime;
	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();
	if (rend == nullptr) {
		return;
	}
	// sin波で点滅させる
	const float value = sinf(m_RecordTime * m_BlinkingSpeed);        // [-1, 1]
	const float normalized = (value + 1.0f) * 0.5f; // [0, 1]
	rend->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, normalized));
}