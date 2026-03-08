#pragma once
#include "RenderComponent.h"
#include "System/DirectXRender.h"
#include <array>

class RoundShadowRenderComponent : public RenderComponent
{
private:
	std::array<ID3D11ShaderResourceView*, 2> m_ShaderResource{ nullptr };
	std::array<ID3D11SamplerState*, 2> m_SamplerState{ nullptr };
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShadowSRV = nullptr;

public:
	RoundShadowRenderComponent(GameObject& obj);
	~RoundShadowRenderComponent() = default;
	void Update() override;

};