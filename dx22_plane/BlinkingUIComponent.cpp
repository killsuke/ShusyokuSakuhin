#include "BlinkingUIComponent.h"
#include "Render2D.h"

using namespace DirectX::SimpleMath;

BlinkingUIComponent::BlinkingUIComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

void BlinkingUIComponent::Update() {
	if (m_blinkingFlag == false) {
		return; // 点滅フラグがfalseなら何もしない
	}
	m_recordTime += m_deltaTime;
	auto rend = p_object->GetComponent<Render2DComponent>();
	if (rend == nullptr) {
		return;
	}
	// sin波で点滅させる
	float value = sinf(m_recordTime * m_blinkingSpeed);        // [-1, 1]
	float normalized = (value + 1.0f) * 0.5f; // [0, 1]
	rend->SetColor(Vector4(1.0f, 1.0f, 1.0f, normalized));
}