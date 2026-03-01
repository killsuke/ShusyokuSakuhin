#pragma once
#include "Component.h"

class DeathBlockComponent : public Component
{
private:

public:
	DeathBlockComponent(GameObject& obj);
	~DeathBlockComponent() = default;
	void Update()override;

};

