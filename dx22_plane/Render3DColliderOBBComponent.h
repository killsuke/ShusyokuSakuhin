#pragma once
#include "Render.h"
class Render3DColliderOBBComponent : public RenderComponent
{
public:
	Render3DColliderOBBComponent(GameObject& obj);
	~Render3DColliderOBBComponent() = default;

	void Update() override;
};
