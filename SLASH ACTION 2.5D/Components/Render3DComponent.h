#pragma once
#include "RenderComponent.h"

class Render3DComponent : public RenderComponent
{
public:
	Render3DComponent(GameObject& obj);
	~Render3DComponent() = default;

	void Update() override;
};
