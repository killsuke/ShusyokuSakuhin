#include "FollowPositionComponent.h"
#include "TransformComponent.h"

using namespace DirectX;

FollowPositionComponent::FollowPositionComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ANIMATION_TRANSFORM"); // ソート番号を設定
}

void FollowPositionComponent::Update() {
	if (m_FollowObj == nullptr) return; // 追従するオブジェクトが設定されていない場合は何もしない
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	TransformComponent* followTrans = m_FollowObj->GetComponent<TransformComponent>();
	if (myTrans == nullptr || followTrans == nullptr) {

		return; // トランスフォームコンポーネントが存在しない場合は何もしない
	}

	// 追従するオブジェクトの位置にオフセットを加えた位置に自分を移動させる
	XMFLOAT3 targetPosition = followTrans->GetPosition() + m_Offset;
	myTrans->SetPosition(targetPosition);
}
