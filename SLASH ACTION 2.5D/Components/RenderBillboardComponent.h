#pragma once
#include "RenderComponent.h"

class RenderBillboardComponent : public RenderComponent
{
private:

public:
	RenderBillboardComponent(GameObject& obj);
	~RenderBillboardComponent() = default;

	void Update() override;
};
