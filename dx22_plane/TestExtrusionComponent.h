#pragma once
#include "Component.h"
class TestExtrusionComponent : public Component
{
private:
	bool isGround = false;

public:
	TestExtrusionComponent(GameObject& obj);

	~TestExtrusionComponent() = default;

	void Update()override;
};

