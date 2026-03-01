#pragma once
#include "RenderComponent.h"

class RenderMask3DComponent :  public RenderComponent
{
public:
	RenderMask3DComponent(GameObject& obj);
	~RenderMask3DComponent() = default;

	void Update() override;
};
