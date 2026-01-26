#pragma once
#include "Render.h"
#include "ParticlesControlComponent.h"
#include <DirectXMath.h>

class RenderLuminescenceBillboardComponent : public RenderComponent
{
private:
	float m_GlowPower = 1.0f; // ‹P“x‚Ì‹­‚³
	float m_GlowRadius = 1.0f; // ‹P‚­”ÍˆÍ‚Ì‘å‚«‚³
	float m_Expand = 1.0f;	// ’¸“_‚ÌL‚°‹ï‡
	DirectX::XMFLOAT2 m_EllipseScale = { 1.0f,1.0f }; // ‘È‰~‚ÌƒXƒP[ƒ‹

public:

	RenderLuminescenceBillboardComponent(GameObject& obj);
	~RenderLuminescenceBillboardComponent() = default;

	void Update() override;

	void SetGlowPower(const float power) { m_GlowPower = power; };
	void SetEllipseScale(const DirectX::XMFLOAT2& scale) { m_EllipseScale = scale; };
	void SetGlowRadius(const float radius) { m_GlowRadius = radius; };
	void SetExpand(const float expand) { m_Expand = expand; };
};