#include "JumpComponent.h"
#include "RigidBodyComponent.h"
#include "input.h"
#include <cmath>
#include <iostream>

using namespace DirectX::SimpleMath;

JumpComponent::JumpComponent(GameObject& obj) :Component(obj) {
	m_sortNum = JUMP;
}

void JumpComponent::Update() {
	bool isPressed = false;
	bool isTrigger = false;

	if (Input::GetKeyTrigger(VK_I) == true || Input::GetButtonTrigger(XINPUT_A) == true) {
		isTrigger = true;
	}

	if (Input::GetKeyPress(VK_I) == true || Input::GetButtonPress(XINPUT_A) == true) {
		isPressed = true;
	}

	JumpAction(isPressed, isTrigger); // isGroundはfalseで初期化

}

void JumpComponent::JumpAction(bool isJumpButtonPressed, bool trigger)
{
	auto rigid = p_object->GetComponent<RigidBodyComponent>();

	Vector3 velocity = rigid->GetVelocity();

	// 地面接触
	if (m_isGround) {
		m_isJumping = false;
		m_time = 0.0f;
		m_firstSpeed = m_jumpPower;

	}

	// ジャンプ時間超過
	if (m_isJumping && m_time > m_maxJumpTime) {
		m_isJumping = false;
	}

	//　ジャンプボタンが押されていて、ジャンプ中でなく、地面の上にいる場合、トリガーもとってさらに厳格に
	if (!m_isJumping && isJumpButtonPressed && m_isGround && trigger == true) {
		m_isJumping = true;
		m_time = 0.0f;
		//std::cout << "Jumping" << std::endl;

	}

	// ジャンプ中の処理
	if (m_isJumping) {
		m_time += m_deltaTime;

	//	std::cout << "Jumping" << std::endl;

		if (isJumpButtonPressed == true && m_time < m_maxJumpTime && m_firstSpeed >= m_velocityIgnore) {
						
			// 値があまりにも小さすぎると無視
			if (m_firstSpeed < m_velocityIgnore) {
				m_firstSpeed = 0.0f;
			}

			//std::cout << m_firstSpeed << std::endl;

			// 速度更新
			velocity.y += m_firstSpeed;
			rigid->SetVelocity(velocity);

			m_firstSpeed = m_firstSpeed * m_attenuationUp;	// 毎フレーム何％ずつ減らす
		}


		if (isJumpButtonPressed == false) {
			m_isJumping = false; // 離したら即終了

			// この速度減速を段階化して、極小、小、中、大、ぐらいで減速させる
			// つまり、あまりにもジャンプが低かったりすると減速しないようにする
			// 速度を減衰させる
			velocity.y -= velocity.y * m_attenuationStop;

			//std::cout << velocity.y << std::endl;

			rigid->SetVelocity(velocity);
		}
	}
}
