#pragma once
#include "Render.h"

class RenderOneSkinAnimation : public RenderComponent
{
public:
	RenderOneSkinAnimation(GameObject& obj);
	~RenderOneSkinAnimation() = default;

	void Update() override;
};
