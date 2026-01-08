#pragma once
#include "Component.h"

enum class PlayerState
{
	NONE,
	MOVE,
	JUMP,
	ATTACK,
	DAMAGE,
	DEAD
};

class PlayerOperationComponent : public Component
{
private:
	float m_RecordTime = 0.0f; // 移動している時間を記録する

	bool m_rightLeft = true; // 左右の移動フラグ、初期は右向き(true:右、false:左)
	bool m_beforeMove = false; // 前回のフレームで移動していたかどうかのフラグ
	bool m_IsMoveFlag = false; // 移動中かどうかのフラグ
	bool m_isJump = false; // ジャンプ中かどうかのフラグ

public:
	PlayerOperationComponent(GameObject& obj);

	~PlayerOperationComponent() = default;

	void Update()override;

	bool GetRightLeft() const { return m_rightLeft; } // 左右の移動フラグ取得
	bool GetMoveFlag() const { return m_IsMoveFlag; } // 移動中かどうかのフラグ取得
	bool GetIsJump() const { return m_isJump; } // ジャンプ中かどうかのフラグ取得
};

