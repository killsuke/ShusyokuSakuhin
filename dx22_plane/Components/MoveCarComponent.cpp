#include "MoveCarComponent.h"
#include "Transform.h"
#include "RigidBodyComponent.h"
#include "GameObjectManager.h"

using namespace DirectX::SimpleMath;

MoveCarComponent::MoveCarComponent(GameObject& obj) :Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("MOVE_CAR"); // ソート番号を設定、仮にMOVE_TERRAINを設置
}

void MoveCarComponent::Update()
{
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	m_PrevCarPos = transform->GetPosition();

	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	rigid->AddForce(DirectX::SimpleMath::Vector3(m_MoveSpeed, 0.0f, 0.0f));

	rigid->UpdateVelocity();

	Vector3 currentPos = transform->GetPosition();

	m_DeltaCarPos = currentPos - m_PrevCarPos;

	m_PrevCarPos = currentPos;
}