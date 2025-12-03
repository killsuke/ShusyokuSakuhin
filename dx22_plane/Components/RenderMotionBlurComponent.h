#pragma once
#include "Render.h"
#include <SimpleMath.h>

class RenderMotionBlurComponent : public RenderComponent
{
private:
	DirectX::SimpleMath::Vector2 m_BlurVelocity = DirectX::SimpleMath::Vector2::Zero;


public:
	RenderMotionBlurComponent(GameObject& obj);
	~RenderMotionBlurComponent() = default;

	void Update()override;

	void SetBlurVelocity(const DirectX::SimpleMath::Vector2& vel) { m_BlurVelocity = vel; };
};