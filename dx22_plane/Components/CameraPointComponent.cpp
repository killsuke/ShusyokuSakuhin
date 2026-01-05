#include "CameraPointComponent.h"
#include "CameraMoveComponent.h"
#include "Manager/GameObjectManager.h"
#include "Collider.h"
#include "RigidBodyComponent.h"
#include "CameraTargetComponent.h"
#include <iostream>
#include <SimpleMath.h>

using namespace DirectX;

CameraPointComponent::CameraPointComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERA_POINT"); // ソート番号を設定
}

void CameraPointComponent::Update() {
	GameObject* camera = GameObjectManager::GameObjectFindName("camera");
	CameraMoveComponent* camMove = camera->GetComponent<CameraMoveComponent>();
	RigidBodyComponent* camRigid = camera->GetComponent<RigidBodyComponent>();

	ColliderComponent* camColl = m_Object->GetComponent<ColliderComponent>();
	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	if (player == nullptr) {
		return;
	}

	ColliderComponent* playerColl = player->GetComponent<ColliderComponent>();

	if (!camMove)
		return;

	// ここで当たり判定でリリースがあればこれ以降の処理を実行
	beforeTouched = afterTouched; // 前に触れたかどうかのフラグを更新

	DirectX::SimpleMath::Vector3 dir = {};
	XMFLOAT3 dir2 = {};
	if (camColl->CheckHit_CubeAndCube_IsTrigger2D_Normal(*camColl, *playerColl, dir)) {

		// プレイヤーが入ってきた方向をセット
		if (m_inserDirection == XMFLOAT3(0.0f, 0.0f, 0.0f)) {
			m_exitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
			dir2 = XMFLOAT3(dir.x, dir.y, dir.z);
			m_inserDirection = dir2;
		}

		m_frame2BeforeDirection = m_beforeDirection; // 2フレーム前のプレイヤーが抜けたベクトルを更新

		m_beforeDirection = dir; // プレイヤーが抜けたベクトルをセット

		afterTouched = true;
	}
	else {
		afterTouched = false;
	}

	// リリースでとる
	if (beforeTouched == true && afterTouched == false) {

		if (m_beforeDirection == XMFLOAT3(0.0f, 0.0f, 0.0f)) {
			m_beforeDirection = m_frame2BeforeDirection;
		}

		// プレイヤーが抜けた方向をセット
		m_exitDirection = m_beforeDirection;

		const XMVECTOR scVec = XMLoadFloat3(&m_scrollDirection);
		const XMVECTOR inVec = XMLoadFloat3(&m_inserDirection);
		const XMVECTOR exVec = XMLoadFloat3(&m_exitDirection);

		const float dirIn1 = XMVectorGetX(XMVector3Dot(scVec,inVec));
		const float dirIn2 = XMVectorGetX(XMVector3Dot(inVec,exVec));

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
		m_inserDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_beforeDirection = XMFLOAT3(0.0f, 0.0f, 0.0f); // 初期化
		m_frame2BeforeDirection = XMFLOAT3(0.0f, 0.0f, 0.0f); // 初期化
	}

	// ポイントから抜けたら処理を行う
	if (m_exitDirection != XMFLOAT3(0.0f, 0.0f, 0.0f)) {

		if (m_nextTargetPoint != nullptr && m_isScrollDir == 1) {
			camMove->SetMoveTarget(*m_nextTargetPoint);
		}
		else if (m_beforeTargetObj != nullptr && m_isScrollDir == -1) {
			camMove->SetMoveTarget(*m_beforeTargetObj);
		}

		m_exitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f); // 初期化
	}
}