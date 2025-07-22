#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"
#include <SimpleMath.h>

class JumpComponent : public Component
{
private:
	float m_jumpHeight = 0.0f; // ジャンプの高さ
	float m_amplitude = 5.0f; // ジャンプの振幅
	float m_time = 0.0f;
	float m_jumpDuration = 0.5f; // 例：ジャンプの全体が0.5秒で完了
	float m_magnification = 2.0f; // ジャンプの加速度の倍率
	float m_firstSpeed = 50.0f;
	bool  m_isJumping = false; // ジャンプ中かどうか

public:
	JumpComponent() = default;
	JumpComponent(float _jumpHeight);
	~JumpComponent() = default;

	void Update()override;
	void JumpAction(RigidBodyComponent& rigid, bool isJumpButtonPressed,bool isGround,bool trigger);

	void SetJumpHeight(const float height) { m_jumpHeight = height; };
	float GetJumpHeight() const { return m_jumpHeight; };

	void StartJump() { m_isJumping = true; };
	void StopJump() { m_isJumping = false; };
};
