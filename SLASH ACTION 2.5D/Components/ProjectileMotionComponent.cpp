// ====================
// 疑似的な投射運動
// ====================

#include "ProjectileMotionComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"

using namespace DirectX;

ProjectileMotionComponent::ProjectileMotionComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("PROJECTILEMOTION"); // ソート番号を設定
}

void ProjectileMotionComponent::Update()
{
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	if (rigid == nullptr) {
		return;
	}

	// 値が０なら何もしない
	if (m_ProjectilePower <= 0.001f || XMVector4Equal(m_ProjectileDirection, XMVectorZero())) {
		return;
	}

	// 飛んでいく方向を決定
	const XMVECTOR dir = XMVectorScale(m_ProjectileDirection,m_ProjectilePower);
	XMFLOAT3 velocity;
	XMStoreFloat3(&velocity,dir);

	// パワーを少しずつ弱める
	m_ProjectilePower -= m_ProjectilePower * 0.1f;

	// クルクル回転させる
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
	trans->AddRotation(m_ProjectileRotation);

	// ここで上への移動を終了させるが、
	// 回転はそのまま維持
	if (m_ProjectilePower < 0.0f) {
		m_ProjectileDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_ProjectilePower = 0.0f;
		m_ProjectileRotation = XMFLOAT3(0.0f,0.0f,0.0f);
		return;
	}

	rigid->AddVelocity(velocity);
}