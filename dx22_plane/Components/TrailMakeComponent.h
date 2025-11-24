#pragma once
#include "Component.h"

class TrailMakeComponent : public Component
{
private:

public:
	
	TrailMakeComponent(GameObject& obj);
	~TrailMakeComponent() = default;

	void Update()override;
};