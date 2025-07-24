#pragma once
#include "Component.h"
class RenderComponent : public Component
{
private:

public:
	RenderComponent(const GameObject& obj);
	~RenderComponent() {};

	void Update() override;

};

