#include "MotionBlurControllerComponent.h"

using namespace DirectX;

MotionBlurControllerComponent::MotionBlurControllerComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_CONTOROL"); // ソート番号を設定
}

void MotionBlurControllerComponent::Update() {

	const TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	if(transform == nullptr) {
		return;
	}
	const XMMATRIX currentMtx = transform->GetWorldMatrix();

	// 前のフレームの行列を更新
	m_PrevMtx = m_PrevMtx2;
	// 最初のフレームだけ現在の行列をセット
	if (XMMatrixIsIdentity(m_PrevMtx)) {
		m_PrevMtx = XMMatrixTranspose(currentMtx);
	}

	// 今回の行列をセット
	m_PrevMtx2 = XMMatrixTranspose(currentMtx);
}