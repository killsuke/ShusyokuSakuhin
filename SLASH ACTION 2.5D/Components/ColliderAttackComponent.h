#pragma once
#include "ColliderComponent.h"

class ColliderAttackComponent :  public ColliderComponent
{
private :

public:

	ColliderAttackComponent(GameObject& obj);
	~ColliderAttackComponent() = default;

	void Update()override;

};
