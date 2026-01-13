#pragma once
#include "Component.h"
#include "RightLeft.h"

enum class PlayerState
{
	NONE,
	MOVE,
	AIR,
	GROUND,
	JUMP,
	ATTACK,
	DAMAGE,
	DEAD
};

class PlayerOperationComponent : public Component
{
private:
	float m_RecordTime = 0.0f; // 移動している時間を記録する
	float m_ChargeTime = 0.0f; // チャージしている時間を記録する
	float m_ChargeCompleteTime = 2.0f; // チャージ完了までの時間

	RightLeft m_CurrentRightLeft = RightLeft::RIGHT; // 現在の向き管理
	PlayerState m_CurrentState = PlayerState::NONE; // プレイヤーの状態管理
	bool m_beforeMove = false; // 前回のフレームで移動していたかどうかのフラグ
	bool m_IsMoveFlag = false; // 移動中かどうかのフラグ
	bool m_IsJump = false; // ジャンプ中かどうかのフラグ
	bool m_IsChargeComplete = false; // チャージ完了フラグ

	GameObject* m_WeaponObject = nullptr; // 武器オブジェクトへのポインタ

	void StateUpdate(); // 状態更新処理
	void ChangeState(const PlayerState& state); // プレイヤーの状態変更
	void Move(const bool right, const bool left, const bool dash, const bool attack, const bool isGround, bool* isMove);
	void Attack(const bool attack,const bool isGround);
	void Charge(const bool charge,const bool attack);
	void CreateChargeSlash();

public:
	PlayerOperationComponent(GameObject& obj);

	~PlayerOperationComponent() = default;

	void Update()override;

	void SetWeaponObject(GameObject* weapon) { m_WeaponObject = weapon; } // 武器オブジェクトのセット
	void SetChargeCompleteTime(const float time) { m_ChargeCompleteTime = time; } // チャージ完了までの時間セット

	RightLeft GetRightLeft() const { return m_CurrentRightLeft; } // 左右の移動フラグ取得
	bool GetMoveFlag() const { return m_IsMoveFlag; } // 移動中かどうかのフラグ取得
	bool GetIsJump() const { return m_IsJump; } // ジャンプ中かどうかのフラグ取得

};

