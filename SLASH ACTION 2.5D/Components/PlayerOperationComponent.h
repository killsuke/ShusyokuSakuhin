#pragma once
#include "FighterComponent.h"
#include "Structs/RightLeft.h"
#include <DirectXMath.h>

enum class PlayerState
{
	NONE,
	MOVE,
	FORCED_MOVE,
	ATTACK,
	CHARGE_SLASH,
	DAMAGE,
	DEAD
};

class PlayerOperationComponent : public Component
{
private:
	float m_BlinkingRecordTime = 0.0f; // 無敵時間中の点滅の時間を記録
	float m_ChargeSlashRecordTime = 0.0f;
	float m_DamageRecordTime = 0.0f; // ダメージを受けてからの時間を記録
	float m_ChargeTime = 0.0f; // チャージしている時間を記録する
	float m_ChargeCompleteTime = 1.5f; // チャージ完了までの時間
	float m_KnockBackPower = 0.0f; // ノックバックの力
	float m_ColorA = 1.0f; // プレイヤーの色のアルファ値
	uint64_t m_ListenerID_HitEvent = 0;
	uint64_t m_ListenerID_FallDamageEvent = 0;

	RightLeft m_CurrentRightLeft = RightLeft::RIGHT; // 現在の向き管理
	RightLeft m_KnockBackRightLeft = RightLeft::NONE; // ノックバックの向き管理
	PlayerState m_CurrentState = PlayerState::NONE; // プレイヤーの状態管理
	bool m_BeforeMove = false; // 前回のフレームで移動していたかどうかのフラグ
	bool m_IsMoveFlag = false; // 移動中かどうかのフラグ
	bool m_IsJump = false; // ジャンプ中かどうかのフラグ
	bool m_IsChargeComplete = false; // チャージ完了フラグ

	DirectX::XMFLOAT3 m_ForceMoveSpeed = { 0.0f,0.0f,0.0f }; // 強制的に移動させるときの速度

	GameObject* m_WeaponObject = nullptr; // 武器オブジェクトへのポインタ
	GameObject* m_ChargeSlashObject = nullptr; // チャージスラッシュオブジェクトへのポインタ

	void StateUpdate(); // 状態更新処理
	void Move(const bool right, const bool left, const bool dash, const bool attack, const bool isGround, bool* isMove);
	void ForcedMove();
	void Attack(const bool attack,const bool isGround);
	void Charge(const bool charge,const bool attack);
	void CreateChargeSlash();
	void FastChageSlash();	// 高速チャージスラッシュ攻撃
	void OnDamageHit(const DamageEvent& event);
	void OnDamageFallHit(const FallDamageEvent& event);
	void CreateSlashEffect();

	void DeadCameraShake();
	void DeadProcess();
	void CreateDeadRing();
	void ChangeResult();

public:
	PlayerOperationComponent(GameObject& obj);

	~PlayerOperationComponent();

	void Update()override;

	void ChangeState(const PlayerState& state); // プレイヤーの状態変更

	void SetWeaponObject(GameObject* weapon) { m_WeaponObject = weapon; } // 武器オブジェクトのセット
	void SetChargeCompleteTime(const float time) { m_ChargeCompleteTime = time; } // チャージ完了までの時間セット
	void SetChargeSlashObject(GameObject* slashObj) { m_ChargeSlashObject = slashObj; } // チャージスラッシュオブジェクトのセット
	void SetForceMoveSpeed(const DirectX::XMFLOAT3& speed) { m_ForceMoveSpeed = speed; } // 強制移動の速度セット

	RightLeft GetRightLeft() const { return m_CurrentRightLeft; } // 左右の移動フラグ取得
	bool GetMoveFlag() const { return m_IsMoveFlag; } // 移動中かどうかのフラグ取得
	bool GetIsJump() const { return m_IsJump; } // ジャンプ中かどうかのフラグ取得
	PlayerState GetPlayerState()const { return m_CurrentState; };
};