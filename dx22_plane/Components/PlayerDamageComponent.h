#pragma once
#include "Component.h"
class PlayerDamageComponent : public Component
{
private:

public:
	PlayerDamageComponent(GameObject& obj);
	~PlayerDamageComponent() = default;
	void Update() override;

};

