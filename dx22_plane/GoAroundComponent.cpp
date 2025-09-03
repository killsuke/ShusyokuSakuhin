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

	m_angle += m_deltaTime * m_rotationSpeed; // Šp“x‚ðXVŠÔŠuŽžŠÔ‚ÉŠî‚Ã‚¢‚ÄŒvŽZ
	float totalAngle = m_angle + m_initialAngle;

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

	transform->SetPosition(centerPos  + rotatedOffset);
}