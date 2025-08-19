#pragma once
#include "Component.h"
class TestExtrusionComponent : public Component
{
private:

public:
	TestExtrusionComponent(GameObject& obj);

	~TestExtrusionComponent() = default;

	void Update()override;
};

