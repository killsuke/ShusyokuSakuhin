#pragma once
#include "Render.h"
class RenderMask3DComponent final :  public RenderComponent
{
public:
	RenderMask3DComponent(GameObject& obj);
	~RenderMask3DComponent() = default;

	void Update() override;
};
