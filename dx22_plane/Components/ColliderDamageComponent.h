#pragma once
#include "Collider.h"

class ColliderDamageComponent : public ColliderComponent
{
private:

public:

	ColliderDamageComponent(GameObject& obj);
	~ColliderDamageComponent() = default;

	void Update() override;

};