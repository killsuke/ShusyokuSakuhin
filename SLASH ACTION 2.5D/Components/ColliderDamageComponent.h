#pragma once
#include "ColliderComponent.h"

class ColliderDamageComponent : public ColliderComponent
{
private:

public:

	ColliderDamageComponent(GameObject& obj);
	~ColliderDamageComponent() = default;

	void Update() override;

};