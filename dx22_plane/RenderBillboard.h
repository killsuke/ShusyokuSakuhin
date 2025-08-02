#pragma once
#include "Render.h"
class RenderBillboardComponent : public RenderComponent
{
public:
	RenderBillboardComponent(GameObject& obj);
	~RenderBillboardComponent() = default;

	void Update() override;
};
