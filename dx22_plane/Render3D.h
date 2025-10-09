#pragma once
#include "Render.h"
class Render3DComponent final : public RenderComponent
{
private:

public:
	Render3DComponent(GameObject& obj);
	~Render3DComponent() = default;

	void Update() override;
};
