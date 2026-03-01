#pragma once
#include "RenderComponent.h"
class Render3DColliderAABBComponent : public RenderComponent
{
public:
	Render3DColliderAABBComponent(GameObject& obj);
	~Render3DColliderAABBComponent() = default;

	void Update() override;
};

