#pragma once
#include "RenderComponent.h"

class Render3DColliderOBBComponent : public RenderComponent
{
public:
	Render3DColliderOBBComponent(GameObject& obj);
	~Render3DColliderOBBComponent();

	void Update() override;
};
