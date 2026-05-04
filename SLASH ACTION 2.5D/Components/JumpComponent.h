#pragma once
#include "RigidBodyComponent.h"
#include <DirectXMath.h>

class JumpComponent final : public Component
{
private:

	float m_jumpPower = 50.0f; // ジャンプの振幅
	float m_time = 0.0f;
	float m_magnification = 2.0f; // ジャンプの加速度の倍率
	float m_firstSpeed = 50.0f;
	float m_maxJumpTime = 0.7f; // ジャンプの最大時間
	float m_velocityIgnore = 0.9f; // ジャンプの最小速度（これ以下は無視する）
	float m_attenuationUp = 0.45f; // ジャンプの上昇中の減衰率（毎フレーム何％ずつ減らすか）
	float m_attenuationStop = 0.5f; // ジャンプの停止したらの減衰率（毎フレーム何％ずつ減らすか）
	bool  m_isJumping = false; // ジャンプ中かどうか
	bool m_isGround = false; // 地面にいるかどうか
	bool m_isCeiling = false; // 天井にいるかどうか
	bool m_beforePress = false; // 前フレームでジャンプボタンが押されていたかどうか
	bool m_jumpPress = false; // ジャンプボタンが押されているかどうか

	void JumpAction(bool isJumpButtonPressed, bool trigger);

public:
	JumpComponent(GameObject& obj);
	~JumpComponent() = default;

	void Update()override;

	void SetJumpPower(const float power) { m_jumpPower = power; };
	float GetJumpPower() const { return m_jumpPower; };

	void SetIsGround(const bool isGround) { m_isGround = isGround; } // 地面にいるかどうかを設定
	bool GetIsGround() const { return m_isGround; } // 地面にいるかどうかを取得

	void SetIsCeiling(const bool isCeiling) { m_isCeiling = isCeiling; } // 天井にいるかどうかを設定
	bool GetIsCeiling() const { return m_isCeiling; } // 天井にいるかどうかを取得

	void StartJump() { m_isJumping = true; };
	void StopJump() { m_isJumping = false; };

	void SetJumpPress(const bool press) { m_jumpPress = press; };

	void SetAttenuationUp(const float attenuation) { m_attenuationUp = attenuation; };
};
