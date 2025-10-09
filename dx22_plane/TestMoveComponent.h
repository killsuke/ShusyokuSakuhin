#pragma once
#include "Component.h"
class TestMoveComponent final : public Component
{
private:
	const float m_deltaTime = 0.016f; // 仮のデルタタイム
	float m_recordTime = 0.0f; // 移動している時間を記録する
	bool m_rightLeft = true; // 左右の移動フラグ、初期は右向き(true:右、false:左)
	bool m_beforeMove = false; // 前回のフレームで移動していたかどうかのフラグ
public:
	TestMoveComponent(GameObject& obj);

	~TestMoveComponent() = default;

	void Update()override;

	bool GetRightLeft() const { return m_rightLeft; } // 左右の移動フラグ取得
};

