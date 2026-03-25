#include "SlashEffectComponent.h"
#include "TransformComponent.h"
#include "RenderLuminescenceBillboardComponent.h"
#include "Manager/TimeManager.h"

using namespace DirectX;

namespace {
	constexpr float SubstructPower = 0.005f;
}

SlashEffectComponent::SlashEffectComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ソート番号を仮置き
}

void SlashEffectComponent::Update() {


	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
	RenderLuminescenceBillboardComponent* rend = m_Object->GetComponent<RenderLuminescenceBillboardComponent>();
	if (trans == nullptr || rend == nullptr) {
		return;
	}

	rend->SetEllipseScale(XMFLOAT2(m_SizeChange.x / 10.0f, m_SizeChange.y / 10.0f));
	rend->SetGlowRadius(power);
	power -= SubstructPower;

	m_RecordTime += TimeManager::GetFixedDeltaTime();
	if (m_RecordTime >= m_RimitTime) {
		m_Object->SetDeleteFg(true); // オブジェクト削除フラグを立てる
	}
}