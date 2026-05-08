#pragma once
#include "RigidBodyComponent.h"
#include <DirectXMath.h>

class JumpComponent final : public Component
{
private:

	float m_JumpPower = 50.0f; // ジャンプの振幅
	float m_RecordTime = 0.0f;
	float m_Magnification = 2.0f; // ジャンプの加速度の倍率
	float m_FirstSpeed = 50.0f;
	float m_MaxJumpTime = 0.7f; // ジャンプの最大時間
	float m_VelocityIgnore = 0.9f; // ジャンプの最小速度（これ以下は無視する）
	float m_AttenuationUp = 0.45f; // ジャンプの上昇中の減衰率（毎フレーム何％ずつ減らすか）
	float m_AttenuationStop = 0.5f; // ジャンプの停止したらの減衰率（毎フレーム何％ずつ減らすか）
	bool  m_IsJumping = false; // ジャンプ中かどうか
	bool m_IsGround = false; // 地面にいるかどうか
	bool m_IsCeiling = false; // 天井にいるかどうか
	bool m_IsBeforePress = false; // 前フレームでジャンプボタンが押されていたかどうか
	bool m_IsJumpPress = false; // ジャンプボタンが押されているかどうか

	void JumpAction(bool isJumpButtonPressed, bool trigger);

public:
	JumpComponent(GameObject& obj);
	~JumpComponent() = default;

	void Update()override;

	void SetJumpPower(const float power) { m_JumpPower = power; };
	float GetJumpPower() const { return m_JumpPower; };

	void SetIsGround(const bool isGround) { m_IsGround = isGround; } // 地面にいるかどうかを設定
	bool GetIsGround() const { return m_IsGround; } // 地面にいるかどうかを取得

	void SetIsCeiling(const bool isCeiling) { m_IsCeiling = isCeiling; } // 天井にいるかどうかを設定
	bool GetIsCeiling() const { return m_IsCeiling; } // 天井にいるかどうかを取得

	void StartJump() { m_IsJumping = true; };
	void StopJump() { m_IsJumping = false; };

	void SetJumpPress(const bool press) { m_IsJumpPress = press; };

	void SetAttenuationUp(const float attenuation) { m_AttenuationUp = attenuation; };
};
