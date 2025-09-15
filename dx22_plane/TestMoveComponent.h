#pragma once
#include "Component.h"
class TestMoveComponent : public Component
{
private:
	bool m_rightLeft = true; // 左右の移動フラグ、初期は右向き(true:右、false:左)
	bool m_beforeMove = false; // 前回のフレームで移動していたかどうかのフラグ
public:
	TestMoveComponent(GameObject& obj);

	~TestMoveComponent() = default;

	void Update()override;

	bool GetRightLeft() const { return m_rightLeft; } // 左右の移動フラグ取得
};

