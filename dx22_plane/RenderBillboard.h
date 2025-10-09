#pragma once
#include "Render.h"
class RenderBillboardComponent final : public RenderComponent
{
private:

public:
	RenderBillboardComponent(GameObject& obj);
	~RenderBillboardComponent() = default;

	void Update() override;
};
