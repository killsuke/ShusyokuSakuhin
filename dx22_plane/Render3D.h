#pragma once
#include "Render.h"
class Render3DComponent : public RenderComponent
{
public:
	Render3DComponent(const GameObject& obj);
	~Render3DComponent() = default;

	void Update() override;
};

