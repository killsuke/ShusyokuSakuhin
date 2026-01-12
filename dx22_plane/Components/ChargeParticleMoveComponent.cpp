#include "ChargeParticleMoveComponent.h"

namespace {
	constexpr float DeltaTime = 0.016f;
}

using namespace DirectX;

ChargeParticleMoveComponent::ChargeParticleMoveComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ソート番号を設定
}

void ChargeParticleMoveComponent::Update() {

	TransformComponent* targetTrans = m_TargetObject->GetComponent<TransformComponent>();
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

	if (myTrans == nullptr || targetTrans == nullptr) {
		return;
	}

	// 移動処理
	XMFLOAT3 targetPos = targetTrans->GetLocalPosition();
	XMFLOAT3 myPos = myTrans->GetLocalPosition();

	XMVECTOR targetVec = XMLoadFloat3(&targetPos);
	XMVECTOR myVec = XMLoadFloat3(&myPos);

	// ターゲットへの方向ベクトルを計算して正規化
	XMVECTOR dir = XMVectorSubtract(targetVec, myVec);
	dir = XMVector3Normalize(dir);

	// 移動量を計算
	XMVECTOR movement = XMVectorScale(dir, m_MoveSpeed * DeltaTime);

	// 新しい位置を計算して設定
	XMVECTOR newPos = XMVectorAdd(myVec, movement);
	XMStoreFloat3(&myPos, newPos);

	// ターゲットに到達したかどうかの判定
	XMVECTOR toTarget = XMVectorSubtract(targetVec, newPos);
	float distanceToTarget = XMVectorGetX(XMVector3Length(toTarget));
	if (distanceToTarget <= m_RimitDistance) {
		m_Object->SetActiveState(ActiveState::ALL_STOP);
		myTrans->SetLocalPosition(m_StartPosition);
		myTrans->SetLocalScale(m_InitScale);
		return;
	}

	myTrans->SetLocalPosition(myPos);
	XMFLOAT3 scale = myTrans->GetLocalScale();
	myTrans->SetLocalScale({ scale.x * 0.9f,scale.y * 0.9f,scale.z });
}

void ChargeParticleMoveComponent::ResetProcess() {

	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	if (myTrans == nullptr) {
		return;
	}
	myTrans->SetLocalPosition(m_StartPosition);
	myTrans->SetLocalScale(m_InitScale);
	m_Object->SetActiveState(ActiveState::ALL_STOP);
}