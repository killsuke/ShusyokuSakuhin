#pragma once
#include "Component.h"
class BossEventComponent : public Component
{
private:
	const float deltaTime = 0.016f;
	float recordTime = 0.0f;
	bool createCompletionFlag = false;

public:
	BossEventComponent(GameObject& obj);
	~BossEventComponent() = default;

	void Update()override;

	void CreateBossWalls();
};