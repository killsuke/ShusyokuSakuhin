#include "FliesMoveComponent.h"
#include "TransformComponent.h"
#include "Manager/TimeManager.h"
#include <random>

using namespace DirectX;

FliesMoveComponent::FliesMoveComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を仮置き
}

void FliesMoveComponent::Update() {

	m_MoveDirection = XMVectorLerp(m_MoveDirection, m_TargetDirection, 0.02f);

	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
	if (trans != nullptr) {
		// 移動処理
		const XMVECTOR dir = XMVectorScale(m_MoveDirection, m_MovePower);
		XMFLOAT3 velocity;
		XMStoreFloat3(&velocity, dir);
		trans->AddPosition(velocity);
	}

	// ハエのように時々動く方向を変える
	if(m_ChangeTargetTime <= 0.0f) {
		// ランダムな方向を生成
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
		XMFLOAT3 randomDir = { dist(gen), dist(gen), 0.0f };
		m_TargetDirection = XMLoadFloat3(&randomDir);
		m_TargetDirection = XMVector3Normalize(m_TargetDirection);
		// 次の目標方向変更までの時間を設定
		m_ChangeTargetTime = dist(gen) * 2.0f + 3.0f; // 1秒から5秒の間
	}

	m_ChangeTargetTime -= TimeManager::GetFixedDeltaTime();
}