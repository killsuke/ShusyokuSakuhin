#pragma once
#include "RenderComponent.h"
#include "ParticlesControlComponent.h"
#include <DirectXMath.h>
#include "System/DirectXRender.h"

class RenderRingLuminescenceBillboardComponent : public RenderComponent
{
private:
	float m_GlowPower = 1.0f; // 輝度の強さ
	float m_GlowRadius = 1.0f; // 輝く範囲の大きさ
	float m_RingRadius = 0.0f;	// リングの範囲の大きさ
	float m_RingWidth = 0.0f;	// リングの幅
	float m_Expand = 1.0f;	// 頂点の広げ具合
	DirectX::XMFLOAT2 m_EllipseScale = { 1.0f,1.0f }; // 楕円のスケール
	EBlendState m_BlendState = EBlendState::BS_ADDITIVE;

public:

	RenderRingLuminescenceBillboardComponent(GameObject& obj);
	~RenderRingLuminescenceBillboardComponent() = default;

	void Update() override;

	void SetGlowPower(const float power) { m_GlowPower = power; };
	void SetEllipseScale(const DirectX::XMFLOAT2& scale) { m_EllipseScale = scale; };
	void SetGlowRadius(const float radius) { m_GlowRadius = radius; };
	void SetRingRadius(const float radius) { m_RingRadius = radius; };
	void SetRingWidth(const float width) { m_RingWidth = width; };
	void SetExpand(const float expand) { m_Expand = expand; };
	void SetBlendState(const EBlendState& state) { m_BlendState = state; };
};