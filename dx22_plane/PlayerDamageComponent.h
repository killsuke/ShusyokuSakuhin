#pragma once
#include "Component.h"
class PlayerDamageComponent final : public Component
{
private:

public:
	PlayerDamageComponent(GameObject& obj);
	~PlayerDamageComponent() = default;
	void Update() override;

};

