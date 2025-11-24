#pragma once
#include "Render.h"
#include <SimpleMath.h>

class RenderBlurComponent : public RenderComponent
{
private:
	DirectX::SimpleMath::Vector2 m_BlurSize = DirectX::SimpleMath::Vector2::Zero;

public:
	RenderBlurComponent(GameObject& obj);
	~RenderBlurComponent() = default;

	void Update() override;

	void SetBlurTextureSize(const DirectX::SimpleMath::Vector2& size) { m_BlurSize = size; };
};