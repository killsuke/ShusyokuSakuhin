#include "CameraPointComponent.h"
#include "CameraMoveComponent.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "RigidBodyComponent.h"
#include "CameraTargetComponent.h"

CameraPointComponent::CameraPointComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("CAMERA_POINT"); // ソート番号を設定
}

void CameraPointComponent::Update() {
	auto camera = GameObjectManager::GameObjectFindName("camera");
	auto camMove = camera->GetComponent<CameraMoveComponent>();
	auto camRigid = camera->GetComponent<RigidBodyComponent>();

	auto camColl = p_object->GetComponent<ColliderComponent>();
	auto player = GameObjectManager::GameObjectFindName("Player");
	if (player == nullptr) {
		return;
	}

	auto playerColl = player->GetComponent<ColliderComponent>();

	if (!camMove)
		return;

	// ここで当たり判定でリリースがあればこれ以降の処理を実行
	beforeTouched = afterTouched; // 前に触れたかどうかのフラグを更新

	DirectX::SimpleMath::Vector3 dir = {};
	if (camColl->CheckHit_CubeAndCube_IsTrigger2D_Normal(*camColl, *playerColl, dir)) {

		// プレイヤーが入ってきた方向をセット
		if (m_inserDirection == DirectX::SimpleMath::Vector3::Zero) {
			m_exitDirection = DirectX::SimpleMath::Vector3::Zero;
			m_inserDirection = dir;
		}

		m_beforeDirection = dir; // プレイヤーが抜けたベクトルをセット

		afterTouched = true;
	}
	else {
		afterTouched = false;
	}

	// リリースでとる
	if (beforeTouched == true && afterTouched == false) {
		// プレイヤーが抜けた方向をセット
		m_exitDirection = m_beforeDirection;

		auto dirIn1 = m_scrollDirection.Dot(m_inserDirection);
		auto dirIn2 = m_inserDirection.Dot(m_exitDirection);

		// プレイヤーが入ってきた方向と抜けた方向が同じなら
		if (dirIn1 == -1.0f && dirIn2 == -1.0f) {
			m_isScrollDir = 1;
		}

		// プレイヤーが抜けた方向と入ってきた方向が違うなら
		if (dirIn1 == 1.0f && dirIn2 == -1.0f) {
			m_isScrollDir = -1;
		}

		if (dirIn2 == 1.0f) {
			m_isScrollDir = 0; // 同じならスクロールしない
		}

		camRigid->ClearVelocity(); // プレイヤーが抜けたらカメラの速度をリセット
		camRigid->ClearForce();
		m_inserDirection = DirectX::SimpleMath::Vector3::Zero;
		m_beforeDirection = DirectX::SimpleMath::Vector3::Zero; // 初期化
	}

	// ポイントから抜けたら処理を行う
	if (m_exitDirection != DirectX::SimpleMath::Vector3::Zero) {

		if (m_nextTargetPoint != nullptr && m_isScrollDir == 1) {
			camMove->SetMoveTarget(*m_nextTargetPoint);
		}
		else if (m_beforeTargetObj != nullptr && m_isScrollDir == -1) {
			camMove->SetMoveTarget(*m_beforeTargetObj);
		}

		m_exitDirection = DirectX::SimpleMath::Vector3::Zero; // 初期化
	}
}