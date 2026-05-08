#include "CameraPointComponent.h"
#include "CameraMoveComponent.h"
#include "Manager/GameObjectManager.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "CameraTargetComponent.h"
#include <iostream>

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
	m_IsBeforeTouched = m_IsAfterTouched; // 前に触れたかどうかのフラグを更新

	XMFLOAT3 dir = {};
	XMFLOAT3 dir2 = {};
	if (camColl->CheckHit_CubeAndCube_IsTrigger2D_Normal(*camColl, *playerColl, dir)) {

		// プレイヤーが入ってきた方向をセット
		if (m_InserDirection == XMFLOAT3(0.0f, 0.0f, 0.0f)) {
			m_ExitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
			dir2 = XMFLOAT3(dir.x, dir.y, dir.z);
			m_InserDirection = dir2;
		}

		m_Frame2BeforeDirection = m_BeforeDirection; // 2フレーム前のプレイヤーが抜けたベクトルを更新

		m_BeforeDirection = dir; // プレイヤーが抜けたベクトルをセット

		m_IsAfterTouched = true;
	}
	else {
		m_IsAfterTouched = false;
	}

	// リリースでとる
	if (m_IsBeforeTouched == true && m_IsAfterTouched == false) {

		if (m_BeforeDirection == XMFLOAT3(0.0f, 0.0f, 0.0f)) {
			m_BeforeDirection = m_Frame2BeforeDirection;
		}

		// プレイヤーが抜けた方向をセット
		m_ExitDirection = m_BeforeDirection;

		const XMVECTOR scVec = XMLoadFloat3(&m_ScrollDirection);
		const XMVECTOR inVec = XMLoadFloat3(&m_InserDirection);
		const XMVECTOR exVec = XMLoadFloat3(&m_ExitDirection);

		const float dirIn1 = XMVectorGetX(XMVector3Dot(scVec,inVec));
		const float dirIn2 = XMVectorGetX(XMVector3Dot(inVec,exVec));

		// プレイヤーが入ってきた方向と抜けた方向が同じなら
		if (dirIn1 == -1.0f && dirIn2 == -1.0f) {
			m_ScrollDir = 1;
		}

		// プレイヤーが抜けた方向と入ってきた方向が違うなら
		if (dirIn1 == 1.0f && dirIn2 == -1.0f) {
			m_ScrollDir = -1;
		}

		if (dirIn2 == 1.0f) {
			m_ScrollDir = 0; // 同じならスクロールしない
		}

		camRigid->ClearVelocity(); // プレイヤーが抜けたらカメラの速度をリセット
		camRigid->ClearForce();
		m_InserDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_BeforeDirection = XMFLOAT3(0.0f, 0.0f, 0.0f); // 初期化
		m_Frame2BeforeDirection = XMFLOAT3(0.0f, 0.0f, 0.0f); // 初期化
	}

	// ポイントから抜けたら処理を行う
	if (m_ExitDirection != XMFLOAT3(0.0f, 0.0f, 0.0f)) {

		if (m_NextTargetPoint != nullptr && m_ScrollDir == 1) {
			camMove->SetMoveTarget(*m_NextTargetPoint);
		}
		else if (m_BeforeTargetObj != nullptr && m_ScrollDir == -1) {
			camMove->SetMoveTarget(*m_BeforeTargetObj);
		}

		m_ExitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f); // 初期化
	}
}