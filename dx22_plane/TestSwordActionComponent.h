#pragma once
#include "Component.h"
class TestSwordActionComponent : public Component
{
private:
	bool m_swordAction = false;
	bool m_beforeDirection = true;	// 前フレームのプレイヤーの向き
	float m_deltaTime = 0.016f;

public:
	TestSwordActionComponent(GameObject& obj);
	~TestSwordActionComponent() = default;

	void Update()override;

	void SwordAction();
};

