#include "ParticlesControlComponent.h"
#include "RenderParticlesComponent.h"
#include "RenderLuminescenceBillboardComponent.h"

using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
}

ParticlesControlComponent::ParticlesControlComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_CONTOROL"); // ソート番号を仮置き
}

void ParticlesControlComponent::Update() {

	RenderLuminescenceBillboardComponent* lumComp = m_Object->GetComponent<RenderLuminescenceBillboardComponent>();
	RenderParticlesComponent* partComp = m_Object->GetComponent<RenderParticlesComponent>();

	// こっちはパーティクルの持続時間とか
	if (partComp == nullptr) {
		return; // どちらかのコンポーネントが存在しない場合は何もしない
	}

	const XMFLOAT4 partColor = partComp->GetColor();
	const float subtractAlpha = m_RecordTime / m_VanishTime;
	const float alpha = 1.0f - subtractAlpha;

	partComp->SetColor({ partColor.x,partColor.y,partColor.z,alpha });

	// 余計な行動をさせないために、念のために全部止める
	if (m_RecordTime > m_VanishTime) {
		m_Object->SetDeleteFg(true);
	}

	m_RecordTime += DeltaTime;

	// こっちはエフェクトの色調整とか
	if (lumComp != nullptr) {

		lumComp->AddColor({ 0.0f,0.0f,0.0f,-subtractAlpha * 0.01f });
	}
}