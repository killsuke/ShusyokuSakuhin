#pragma once
#include "Component.h"
class TestMoveComponent : public Component
{
	public:
	TestMoveComponent(GameObject& obj);

	~TestMoveComponent() = default;

	void Update()override;

};

