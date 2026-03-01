#include "VectorMoveComponent.h"
#include "TransformComponent.h"

using namespace DirectX;

VectorMoveComponent::VectorMoveComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("PROJECTILEMOTION"); // ソート番号を設定
}

void VectorMoveComponent::Update()
{
	// 値が０なら何もしない
	if (m_MovePower == 0.0f || XMVector4Equal(m_MoveDirection, XMVectorZero())) {
		return;
	}

	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
	if(trans == nullptr){
		return;
	}

	// 飛んでいく方向を決定
	const XMVECTOR dir = XMVectorScale(m_MoveDirection, m_MovePower);
	XMFLOAT3 velocity;
	XMStoreFloat3(&velocity, dir);

	trans->AddRotation(m_RotationValue);

	trans->AddPosition(velocity);
}