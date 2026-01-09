#pragma once
#include "Render.h"
#include "ParticlesControlComponent.h"
#include <DirectXMath.h>

class RenderLuminescenceBillboardComponent : public RenderComponent
{
private:
	float m_GlowPower = 1.0f; // ‹P“x‚Ì‹­‚³
	DirectX::XMFLOAT2 m_EllipseScale = { 1.0f,1.0f }; // ‘È‰~‚ÌƒXƒP[ƒ‹
	float m_GlowRadius = 1.0f; // ‹P‚­”ÍˆÍ‚Ì‘å‚«‚³

public:

	RenderLuminescenceBillboardComponent(GameObject& obj);
	~RenderLuminescenceBillboardComponent() = default;

	void Update() override;

	void SetGlowPower(const float power) { m_GlowPower = power; };
	void SetEllipseScale(const DirectX::XMFLOAT2& scale) { m_EllipseScale = scale; };
	void SetGlowRadius(const float radius) { m_GlowRadius = radius; };
};