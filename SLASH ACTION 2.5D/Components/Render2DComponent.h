#pragma once
#include "RenderComponent.h"

class Render2DComponent : public RenderComponent
{
public:
	Render2DComponent(GameObject& obj);
	~Render2DComponent() = default;

	void Update() override;
};
