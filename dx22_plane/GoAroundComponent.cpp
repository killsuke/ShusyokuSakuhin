#include "GoAroundComponent.h"
#include "Transform.h"
#include <SimpleMath.h>
#include "GameObjectManager.h"
#include <iostream>

using namespace DirectX::SimpleMath;

GoAroundComponent::GoAroundComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ソート番号を設定
}

void GoAroundComponent::Update()
{

	auto transform = p_object->GetComponent<TransformComponent>();
	auto rollingObjPos = transform->GetPosition();

	auto centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	auto centerPos = centerTrans->GetPosition();

	m_angle += m_deltaTime * m_rotationSpeed; // 角度を更新間隔時間に基づいて計算
	float totalAngle = m_angle + m_initialAngle;

	Vector3 rotatedOffset = Vector3(
		cosf(totalAngle) * m_radius,
		sinf(totalAngle) * m_radius,
		0.0f
	);

	transform->SetPosition(centerPos  + rotatedOffset);
}