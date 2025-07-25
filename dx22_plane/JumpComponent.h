#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"
#include <SimpleMath.h>

class JumpComponent : public Component
{
private:
	float m_jumpHeight = 0.0f; // ジャンプの高さ
	float m_jumpPower = 5.0f; // ジャンプの振幅
	float m_time = 0.0f;
	float m_jumpDuration = 0.5f; // 例：ジャンプの全体が0.5秒で完了
	float m_magnification = 2.0f; // ジャンプの加速度の倍率
	float m_firstSpeed = 50.0f;
	float m_maxJumpTime = 0.7f; // ジャンプの最大時間
	float m_velocityIgnore = 0.9f; // ジャンプの最小速度（これ以下は無視する）
	float m_attenuationUp = 0.7f; // ジャンプの上昇中の減衰率（毎フレーム何％ずつ減らすか）
	float m_attenuationStop = 0.5f; // ジャンプの停止したらの減衰率（毎フレーム何％ずつ減らすか）
	const float m_deltaTime = 0.016f;
	bool  m_isJumping = false; // ジャンプ中かどうか

public:
	JumpComponent() = default;
	JumpComponent(GameObject& obj);
	~JumpComponent() = default;

	void Update()override;
	void JumpAction(bool isJumpButtonPressed,bool isGround,bool trigger);

	void SetJumpPower(const float power) { m_jumpPower = power; };
	float GetJumpPower() const { return m_jumpPower; };

	void StartJump() { m_isJumping = true; };
	void StopJump() { m_isJumping = false; };
};
