#include "FollowPositionComponent.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

FollowPositionComponent::FollowPositionComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("ANIMATION_TRANSFORM"); // ソート番号を設定
}

void FollowPositionComponent::Update() {
	if (p_followObj == nullptr) return; // 追従するオブジェクトが設定されていない場合は何もしない
	auto myTrans = m_Object->GetComponent<TransformComponent>();
	auto followTrans = p_followObj->GetComponent<TransformComponent>();
	if (myTrans == nullptr || followTrans == nullptr) return; // トランスフォームコンポーネントが存在しない場合は何もしない
	// 追従するオブジェクトの位置にオフセットを加えた位置に自分を移動させる
	Vector3 targetPosition = followTrans->GetPosition() + m_offset;
	myTrans->SetPosition(targetPosition);
}
