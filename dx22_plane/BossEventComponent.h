#pragma once
#include "Component.h"
class BossEventComponent final : public Component
{
private:
	const float deltaTime = 0.016f;
	float recordTime = 0.0f;
	bool createCompletionFlag = false;
	GameObject* m_boss = nullptr;

public:
	BossEventComponent(GameObject& obj);
	~BossEventComponent() = default;

	void Update()override;

	void CreateBossWalls();
};