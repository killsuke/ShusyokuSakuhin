#pragma once
#include "Component.h"
class DoorFadeComponent : public Component
{
private:

public:
	DoorFadeComponent(GameObject& obj);
	~DoorFadeComponent() = default;
	virtual void Update() override;
};

