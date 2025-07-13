#include "JumpComponent.h"

void JumpComponent::Update() {}

void JumpComponent::JumpAction(DirectX::XMFLOAT3& pos) {
	// 0.016fをdeltaTimeとして使用
	float t = 0.016f; // 0～1 のジャンプ進行度
	float jumpOffset = amplitude * sinf(t * DirectX::XM_PI);

	// 現在の位置のY座標にジャンプの高さを加算
	pos.y += jumpOffset;
}

