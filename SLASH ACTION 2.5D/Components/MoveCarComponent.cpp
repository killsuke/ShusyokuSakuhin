#include "MoveCarComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "Manager/GameObjectManager.h"

using namespace DirectX;

MoveCarComponent::MoveCarComponent(GameObject& obj) :Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("MOVE_CAR"); // ソート番号を設定、仮にMOVE_TERRAINを設置
}

void MoveCarComponent::Update()
{
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	m_PrevCarPos = transform->GetPosition();

	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	rigid->AddForce(XMFLOAT3(m_MoveSpeed, 0.0f, 0.0f));

	rigid->UpdateVelocity();

	const XMFLOAT3 currentPos = transform->GetPosition();

	m_DeltaCarPos = currentPos - m_PrevCarPos;

	m_PrevCarPos = currentPos;
}