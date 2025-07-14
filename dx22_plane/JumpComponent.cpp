#include "JumpComponent.h"
#include <cmath>
#include <iostream>

JumpComponent::JumpComponent(float _jumpHeight) :m_amplitude(_jumpHeight) {

}

void JumpComponent::Update() {}

void JumpComponent::JumpAction(RigidBodyComponent& rigid, bool isJumpButtonPressed, bool isGround,bool trigger)
{
    const float deltaTime = 0.016f;
    const float maxJumpTime = 1.0f; // 長押しで最大0.4秒滞空
    DirectX::XMFLOAT3 velocity = rigid.GetVelocity();

    if (isGround) {
        m_isJumping = false;
        m_time = 0.0f;
    }

    if (m_isJumping && m_time > maxJumpTime) {
        m_isJumping = false;
    }

    if (!m_isJumping && isJumpButtonPressed && isGround && trigger == true) {
        m_isJumping = true;
        m_time = 0.0f;
        velocity.y = 30.0f; // 初速を強め
        rigid.SetVelocity(velocity);
    }

    if (m_isJumping) {
        m_time += deltaTime;

        if (isJumpButtonPressed && m_time < maxJumpTime) {
            float t = m_time / maxJumpTime;
            float addVelocity = (1.0f - t) * 200.0f * (deltaTime); // イージング風加速
            
            std::cout << addVelocity << std::endl;
            
            velocity = rigid.GetVelocity();
            velocity.y += addVelocity;
            rigid.SetVelocity(velocity);
        }

        if (!isJumpButtonPressed) {
            m_isJumping = false; // 離したら即終了
        }
    }
}
