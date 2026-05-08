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
	if(m_IsBeforePress == false && m_IsJumpPress == true) {
		isJumpButtonTrigger = true;
	}

	JumpAction(m_IsJumpPress, isJumpButtonTrigger); // isGroundはfalseで初期化

	m_IsBeforePress = m_IsJumpPress;
}

// ジャンプ処理
void JumpComponent::JumpAction(bool isJumpButtonPressed, bool trigger)
{
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	
	if(rigid == nullptr) {
		return;
	}

	XMFLOAT3 velocity = rigid->GetVelocity();

	// 地面接触
	if (m_IsGround) {
		m_IsJumping = false;
		m_RecordTime = 0.0f;
		m_FirstSpeed = m_JumpPower;
	}

	// ジャンプ時間超過
	if (m_IsJumping && m_RecordTime > m_MaxJumpTime) {
		m_IsJumping = false;
	}

	//　ジャンプボタンが押されていて、ジャンプ中でなく、地面の上にいる場合、トリガーもとってさらに厳格に
	if (!m_IsJumping && isJumpButtonPressed && m_IsGround && trigger == true) {
		m_IsJumping = true;
		m_RecordTime = 0.0f;
	}

	// ジャンプ中の処理
	if (m_IsJumping) {

		const float deltaTime = TimeManager::GetFixedDeltaTime();

		m_RecordTime += deltaTime;

		if (isJumpButtonPressed == true && m_RecordTime < m_MaxJumpTime && m_FirstSpeed >= m_VelocityIgnore) {
						
			// 値があまりにも小さすぎると無視
			if (m_FirstSpeed < m_VelocityIgnore) {
				m_FirstSpeed = 0.0f;
			}

			velocity.y += m_FirstSpeed;

			// 速度更新
			rigid->SetVelocity(velocity);

			m_FirstSpeed -= m_FirstSpeed * m_AttenuationUp;	// 毎フレーム何％ずつ減らす
		}

		// ジャンプ中に天井に接触するか
		// ボタンを離すとジャンプを終了
		if (isJumpButtonPressed == false || m_IsCeiling == true) {
			m_IsJumping = false; // 離したら即終了

			// この速度減速を段階化して、極小、小、中、大、ぐらいで減速させる
			// つまり、あまりにもジャンプが低かったりすると減速しないようにする
			// 速度を減衰させる
			 velocity.y -= velocity.y * m_AttenuationStop;

			rigid->SetVelocity(velocity);
		}
	}
}
