#pragma once
#include "RenderComponent.h"

class RenderTextureLuminescenceComponent : public RenderComponent
{
private:
	float m_GlowPower = 1.0f; // ‹P“x‚Ì‹­‚³
	float m_GlowRadius = 1.0f; // ‹P‚­”ÍˆÍ‚Ì‘å‚«‚³
	float m_Expand = 1.0f;	// ’¸“_‚ÌL‚°‹ï‡
	DirectX::XMFLOAT2 m_EllipseScale = { 1.0f,1.0f }; // ‘È‰~‚ÌƒXƒP[ƒ‹
	DirectX::XMFLOAT4 m_GlowColor = { 1.0f,1.0f,1.0f,1.0f }; // ‹P‚«‚ÌF
	EBlendState m_BlendState = EBlendState::BS_ADDITIVE;

public:
	RenderTextureLuminescenceComponent(GameObject& obj);
	~RenderTextureLuminescenceComponent() = default;
	void Update() override;

	void SetGlowPower(const float power) { m_GlowPower = power; };
	void SetEllipseScale(const DirectX::XMFLOAT2& scale) { m_EllipseScale = scale; };
	void SetGlowRadius(const float radius) { m_GlowRadius = radius; };
	void SetGlowColor(const DirectX::XMFLOAT4& color) { m_GlowColor = color; };
	void SetBlendState(const EBlendState& state) { m_BlendState = state; };
};