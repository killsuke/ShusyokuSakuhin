#include "MotionBlurControllerComponent.h"

using namespace DirectX;

MotionBlurControllerComponent::MotionBlurControllerComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("MOTIONBLUR"); // ソート番号を設定
}

void MotionBlurControllerComponent::Update() {

	const TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	if(transform == nullptr) {
		return;
	}
	XMMATRIX currentMtx = transform->GetWorldMatrix();

	// 前のフレームの行列を更新
	m_PrevMtx = m_PrevMtx2;
	// 最初のフレームだけ現在の行列をセット
	if (XMMatrixIsIdentity(m_PrevMtx)) {
		m_PrevMtx = XMMatrixTranspose(currentMtx);
	}

	m_PrevMtx2 = XMMatrixTranspose(currentMtx);
}