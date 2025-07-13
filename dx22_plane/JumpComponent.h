#pragma once
#include "Component.h"
#include <SimpleMath.h>

class JumpComponent : public Component
{
private:
	float jumpHeight = 0.0f; // ジャンプの高さ
	float amplitude = 5.0f; // ジャンプの振幅
	bool isJumping = false; // ジャンプ中かどうか

public:
	JumpComponent() = default;
	~JumpComponent() = default;

	void Update()override;
	void JumpAction(DirectX::XMFLOAT3& pos);

	void SetJumpHeight(const float height) { jumpHeight = height; };
	float GetJumpHeight() const { return jumpHeight; };

	void StartJump() { isJumping = true; };
	void StopJump() { isJumping = false; };
};
