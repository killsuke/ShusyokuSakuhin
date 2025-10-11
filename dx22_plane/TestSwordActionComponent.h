#pragma once
#include "Component.h"
class TestSwordActionComponent : public Component
{
private:
	bool m_swordAction = false;
	bool m_beforeDirection = true;	// 前フレームのプレイヤーの向き
	bool m_rightLeft = true; // プレイヤーの向き、trueが右、falseが左
	float m_deltaTime = 0.016f;
	GameObject* m_holder = nullptr; // 持ち主のオブジェクト

public:
	TestSwordActionComponent(GameObject& obj);
	~TestSwordActionComponent() = default;

	void Update()override;

	void CreateSwordEffect(); // 剣のエフェクトを生成

	void SwordAction();
	void SetHolder(GameObject* holder) { m_holder = holder; };
	GameObject* GetHolder() { return m_holder; };
};

