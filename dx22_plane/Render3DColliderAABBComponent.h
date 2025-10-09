#pragma once
#include "Render.h"
class Render3DColliderAABBComponent final : public RenderComponent
{
public:
	Render3DColliderAABBComponent(GameObject& obj);
	~Render3DColliderAABBComponent() = default;

	void Update() override;
};

