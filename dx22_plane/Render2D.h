#pragma once
#include "Render.h"
class Render2DComponent final : public RenderComponent
{
public:
	Render2DComponent(GameObject& obj);
	~Render2DComponent() = default;

	void Update() override;
};
