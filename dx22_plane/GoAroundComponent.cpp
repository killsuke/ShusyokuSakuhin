#include "GoAroundComponent.h"
#include "Transform.h"
#include <SimpleMath.h>
#include "GameObjectManager.h"
#include <iostream>

using namespace DirectX::SimpleMath;

GoAroundComponent::GoAroundComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void GoAroundComponent::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();
	auto rollingObjPos = transform->GetPosition();

	auto centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	auto centerPos = centerTrans->GetPosition();

	// ‰ñ“]‚Ì‚Ý‚ð’âŽ~AˆÊ’uXV‚Í‚·‚é
	if (m_rollingActive == true) {
		// ŽžŒv‰ñ‚è‚©”½ŽžŒv‰ñ‚è‚©
		if (m_clockwise == true) {
			m_nowAngleRadian -= m_deltaTime * m_rotationSpeed; // Šp“x‚ðXVŠÔŠuŽžŠÔ‚ÉŠî‚Ã‚¢‚ÄŒvŽZ
		}
		else {
			m_nowAngleRadian += m_deltaTime * m_rotationSpeed; // Šp“x‚ðXVŠÔŠuŽžŠÔ‚ÉŠî‚Ã‚¢‚ÄŒvŽZ
		}
	}

	// ”½“]—v‹‚ª‚ ‚ê‚ÎAŒ»Ý‚ÌŠp“x‚ð”½“]
	if (m_flipRequested == true) {

		m_nowAngleRadian = -m_nowAngleRadian;
		m_flipRequested = false;
	}

	float totalAngle = m_nowAngleRadian + m_initialAngleRadian;

	m_nowAngleDegree = totalAngle * (180.0f / DirectX::XM_PI);	// ƒ‰ƒWƒAƒ“ ¨ ƒfƒBƒOƒŠ[

	// Œ»Ý‚ÌˆÊ’u == Šp“x‚Æ‚µ‚Ä‘¨‚¦‚½‚¢‚Ì‚ÅA–ß‚µˆ—‚ð“ü‚ê‚é
	if ((std::abs(m_nowAngleDegree) + std::abs(m_initialAngleDegree)) > 360.0f && m_clockwise == true) {	// ŽžŒv‰ñ‚è
		m_nowAngleRadian = 0.0f;
	}
	else if ((std::abs(m_nowAngleDegree) - std::abs(m_initialAngleDegree)) > 360.0f && m_clockwise == false) {	// ”½ŽžŒv‰ñ‚è
		m_nowAngleRadian = 0.0f;
	}

	Vector3 rotatedOffset = Vector3(
		cosf(totalAngle) * m_radius,
		sinf(totalAngle) * m_radius,
		0.0f
	);

	Vector3 directionToCenter = centerPos - (centerPos + rotatedOffset);
	directionToCenter.Normalize();

	// ‚yŽ²‰ñ“]i‚Q‚c•½–Êãj‚ð‘z’è‚µ‚Ä‚xŽ²‚ðã‚É‚µ‚½‚¢ê‡
	float angleToCenter = atan2f(directionToCenter.y, directionToCenter.x);

	transform->SetRotation(Vector3(0.0f, 0.0f, DirectX::XMConvertToDegrees(angleToCenter)));

	transform->SetPosition(centerPos + rotatedOffset);

	//std::cout << transform->GetRotation().z << std::endl;
}