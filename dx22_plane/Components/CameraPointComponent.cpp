#include "CameraPointComponent.h"
#include "CameraMoveComponent.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "RigidBodyComponent.h"
#include "CameraTargetComponent.h"
#include <iostream>

using namespace DirectX::SimpleMath;

CameraPointComponent::CameraPointComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("CAMERA_POINT"); // ソート番号を設定
}

void CameraPointComponent::Update() {
	auto camera = GameObjectManager::GameObjectFindName("camera");
	auto camMove = camera->GetComponent<CameraMoveComponent>();
	auto camRigid = camera->GetComponent<RigidBodyComponent>();

	auto camColl = m_Object->GetComponent<ColliderComponent>();
	auto player = GameObjectManager::GameObjectFindName("Player");
	if (player == nullptr) {
		return;
	}

	auto playerColl = player->GetComponent<ColliderComponent>();

	if (!camMove)
		return;

	// ここで当たり判定でリリースがあればこれ以降の処理を実行
	beforeTouched = afterTouched; // 前に触れたかどうかのフラグを更新

	Vector3 dir = {};
	if (camColl->CheckHit_CubeAndCube_IsTrigger2D_Normal(*camColl, *playerColl, dir)) {

		// プレイヤーが入ってきた方向をセット
		if (m_inserDirection == Vector3::Zero) {
			m_exitDirection = Vector3::Zero;
			m_inserDirection = dir;
		//	std::cout << "IN：" << dir.x << "：" << dir.y << "：" << dir.z << std::endl;
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

		//std::cout << "OUT：" << m_beforeDirection.x << "：" << m_beforeDirection.y << "：" << m_beforeDirection.z << std::endl;

		if (m_beforeDirection == Vector3::Zero) {
			m_beforeDirection = m_frame2BeforeDirection;
		}

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
		m_inserDirection = Vector3::Zero;
		m_beforeDirection = Vector3::Zero; // 初期化
		m_frame2BeforeDirection = Vector3::Zero; // 初期化
	}

	// ポイントから抜けたら処理を行う
	if (m_exitDirection != Vector3::Zero) {

		if (m_nextTargetPoint != nullptr && m_isScrollDir == 1) {
			camMove->SetMoveTarget(*m_nextTargetPoint);
		}
		else if (m_beforeTargetObj != nullptr && m_isScrollDir == -1) {
			camMove->SetMoveTarget(*m_beforeTargetObj);
		}

		m_exitDirection = Vector3::Zero; // 初期化
	}
}