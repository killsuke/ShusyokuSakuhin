#pragma once
#include "RenderComponent.h"
#include <DirectXMath.h>

class RenderBlurComponent : public RenderComponent
{
private:
	DirectX::XMFLOAT2 m_BlurSize = DirectX::XMFLOAT2();

public:
	RenderBlurComponent(GameObject& obj);
	~RenderBlurComponent() = default;

	void Update() override;

	void SetBlurTextureSize(const DirectX::SimpleMath::Vector2& size) { m_BlurSize = size; };
};