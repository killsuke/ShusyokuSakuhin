#include "JumpComponent.h"
#include "RigidBodyComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "Manager/TimeManager.h"
#include <cmath>
#include <iostream>

using namespace DirectX;

JumpComponent::JumpComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("JUMP"); // ソート番号を設定
}

void JumpComponent::Update() {
	
	bool isJumpButtonTrigger = false;
	if(m_beforePress == false && m_jumpPress == true) {
		isJumpButtonTrigger = true;
	}

	JumpAction(m_jumpPress, isJumpButtonTrigger); // isGroundはfalseで初期化

	m_beforePress = m_jumpPress;
}

void JumpComponent::JumpAction(bool isJumpButtonPressed, bool trigger)
{
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	
	if(rigid == nullptr) {
		return; // RigidBodyComponentがない場合は何もしない
	}

	XMFLOAT3 velocity = rigid->GetVelocity();

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
	}

	// ジャンプ中の処理
	if (m_isJumping) {

		const float deltaTime = TimeManager::GetFixedDeltaTime();

		m_time += deltaTime;

		if (isJumpButtonPressed == true && m_time < m_maxJumpTime && m_firstSpeed >= m_velocityIgnore) {
						
			// 値があまりにも小さすぎると無視
			if (m_firstSpeed < m_velocityIgnore) {
				m_firstSpeed = 0.0f;
			}

			velocity.y += m_firstSpeed;

			// 速度更新
			rigid->SetVelocity(velocity);

			m_firstSpeed -= m_firstSpeed * m_attenuationUp;	// 毎フレーム何％ずつ減らす
		}

		// ジャンプ中に天井に接触するか
		// ボタンを離すとジャンプを終了
		if (isJumpButtonPressed == false || m_isCeiling == true) {
			m_isJumping = false; // 離したら即終了

			// この速度減速を段階化して、極小、小、中、大、ぐらいで減速させる
			// つまり、あまりにもジャンプが低かったりすると減速しないようにする
			// 速度を減衰させる
			 velocity.y -= velocity.y * m_attenuationStop;

			rigid->SetVelocity(velocity);
		}
	}
}
