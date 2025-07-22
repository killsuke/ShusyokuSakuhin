#include "JumpComponent.h"
#include <cmath>
#include <iostream>

JumpComponent::JumpComponent(float _jumpHeight) :m_amplitude(_jumpHeight) {

}

void JumpComponent::Update() {}

void JumpComponent::JumpAction(RigidBodyComponent& rigid, bool isJumpButtonPressed, bool isGround, bool trigger)
{
	const float deltaTime = 0.016f;
	const float maxJumpTime = 0.7f; // 長押しで最大0.4秒滞空
	DirectX::XMFLOAT3 velocity = rigid.GetVelocity();

	// 地面接触
	if (isGround) {
		m_isJumping = false;
		m_time = 0.0f;
		m_firstSpeed = 50.0f;
	}

	// ジャンプ時間超過
	if (m_isJumping && m_time > maxJumpTime) {
		m_isJumping = false;
	}

	//　ジャンプボタンが押されていて、ジャンプ中でなく、地面の上にいる場合、トリガーもとってさらに厳格に
	if (!m_isJumping && isJumpButtonPressed && isGround && trigger == true) {
		m_isJumping = true;
		m_time = 0.0f;
	}

	// ジャンプ中の処理
	if (m_isJumping) {
		m_time += deltaTime;

		if (isJumpButtonPressed == true && m_time < maxJumpTime && m_firstSpeed >= 0.9f) {
						
			// 値があまりにも小さすぎると無視
			if (m_firstSpeed < 0.9f) {
				m_firstSpeed = 0.0f;
			}

			std::cout << m_firstSpeed << std::endl;

			// 速度更新
			velocity.y += m_firstSpeed;
			rigid.SetVelocity(velocity);

			m_firstSpeed = m_firstSpeed * 0.7f;	// 減らす
		}


		if (isJumpButtonPressed == false) {
			m_isJumping = false; // 離したら即終了

			// この速度減速を段階化して、極小、小、中、大、ぐらいで減速させる
			// つまり、あまりにもジャンプが低かったりすると減速しないようにする
			// 速度を減衰させる
			velocity.y -= velocity.y * 0.5f;

			//std::cout << velocity.y << std::endl;

			rigid.SetVelocity(velocity);
		}
	}
}
