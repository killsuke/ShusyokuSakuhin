#pragma once
#include "Component.h"
class BossEventComponent : public Component
{
private:
	float m_RecordTime = 0.0f;
	bool createCompletionFlag = false;
	bool m_IsBossDied = false;
	GameObject* m_boss = nullptr;

public:
	BossEventComponent(GameObject& obj);
	~BossEventComponent() = default;

	void Update()override;

	void CreateBossWalls();
};