#pragma once
#include "RenderComponent.h"

class RenderOneSkinAnimation : public RenderComponent
{
private:
	VertexBuffer<AnimationVertex> m_AnimationVertexBuffer = {};

public:
	RenderOneSkinAnimation(GameObject& obj);
	~RenderOneSkinAnimation() = default;

	void Update() override;
};
