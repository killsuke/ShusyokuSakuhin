#pragma once
#include "RenderComponent.h"

class RenderTextureLuminescenceComponent : public RenderComponent
{
private:
	float m_GlowPower = 1.0f; // ãPìxÇÃã≠Ç≥
	float m_GlowRadius = 1.0f; // ãPÇ≠îÕàÕÇÃëÂÇ´Ç≥
	float m_Expand = 1.0f;	// í∏ì_ÇÃçLÇ∞ãÔçá
	DirectX::XMFLOAT2 m_EllipseScale = { 1.0f,1.0f }; // ë»â~ÇÃÉXÉPÅ[Éã
	DirectX::XMFLOAT4 m_GlowColor = { 1.0f,1.0f,1.0f,1.0f }; // ãPÇ´ÇÃêF
	EBlendState m_BlendState = EBlendState::BS_ADDITIVE;
	std::unique_ptr<Shader> m_GlowShader = nullptr;

public:
	RenderTextureLuminescenceComponent(GameObject& obj);
	~RenderTextureLuminescenceComponent() = default;
	void Update() override;

	void SetGlowPower(const float power) { m_GlowPower = power; };
	void SetEllipseScale(const DirectX::XMFLOAT2& scale) { m_EllipseScale = scale; };
	void SetGlowRadius(const float radius) { m_GlowRadius = radius; };
	void SetExpand(const float expand) { m_Expand = expand; };
	void SetGlowColor(const DirectX::XMFLOAT4& color) { m_GlowColor = color; };
	void SetBlendState(const EBlendState& state) { m_BlendState = state; };

	void AddGlowColor(const DirectX::XMFLOAT4& color) {
		m_GlowColor.x += color.x;
		m_GlowColor.y += color.y;
		m_GlowColor.z += color.z;
		m_GlowColor.w += color.w;
	};
	void AddGlowColor_R(const float r) { m_GlowColor.x += r; };
	void AddGlowColor_G(const float g) { m_GlowColor.y += g; };
	void AddGlowColor_B(const float b) { m_GlowColor.z += b; };
	void AddGlowColor_A(const float a) { m_GlowColor.w += a; };
};