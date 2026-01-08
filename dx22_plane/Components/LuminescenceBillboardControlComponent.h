#pragma once
#include "Component.h"

class LuminescenceBillboardControlComponent : public Component
{
private:

public:
	LuminescenceBillboardControlComponent(GameObject& obj);
	~LuminescenceBillboardControlComponent() = default;
	void Update() override;

};

