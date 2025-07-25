#pragma once
#include "Component.h"
class TestExtrusionComponent : public Component
{
public:
	TestExtrusionComponent(GameObject& obj);

	~TestExtrusionComponent() = default;

	void Update()override;
};

