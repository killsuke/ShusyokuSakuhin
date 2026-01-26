#pragma once
#include "Collider.h"

class ColliderAttackComponent :  public ColliderComponent
{
private :

public:

	ColliderAttackComponent(GameObject& obj);
	~ColliderAttackComponent() = default;

	void Update()override;

};
