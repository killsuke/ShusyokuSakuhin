#include "CameraPointComponent.h"
#include "CameraMoveComponent.h"
#include "GameObjectManager.h"
#include "Collider.h"

CameraPointComponent::CameraPointComponent(GameObject& obj): Component(obj)
{
	m_sortNum = CAMERA_POINT;
	cp = CAMERA_NONE; // 初期はCHASEカメラ
}

void CameraPointComponent::Update() {
	auto camera = GameObjectManager::GameObjectFindName("camera");
	auto camMove = camera->GetComponent<CameraMoveComponent>();
	auto camColl = camera->GetComponent<ColliderComponent>();
	auto player = GameObjectManager::GameObjectFindName("Player");
	auto playerColl = player->GetComponent<ColliderComponent>();

	if (!camMove)
		return;

	// ここで当たり判定でリリースがあればこれ以降の処理を実行

	DirectX::SimpleMath::Vector3 dir = {};
	if (camColl->CheckHit_CubeAndCube_IsTrigger2D_Normal(*camColl, *playerColl, dir)) {

		// プレイヤーが入ってきた方向をセット
		if(m_inserDirection != DirectX::SimpleMath::Vector3::Zero) {
			m_exitDirection = DirectX::SimpleMath::Vector3::Zero;
			m_inserDirection = dir;
		}
	}

	// プレイヤーが抜けた方向をセット
	// あとでつくる


	switch (cp)
	{
	case CAMERA_NONE:

		break;
	case CHASE:
		camMove->SetCameraPattern(CHASE);
		break;
	case SPRING:
		// プレイヤーが入ってきた方向と抜けた方向が、
		// 同じならm_targetPosで指定した位置にスクロール
		// 違うなら、m_beforePointが持つ指定した位置にスクロール
		
		// m_targetObjがもつオブジェクトのポジションにする？
		// と思ったけど面倒だから直のポイントでいい
		camMove->SetCameraPattern(SPRING_CHASE);

		// 入ってきた面と出た面が違うなら
		if (m_targetObj != nullptr) {
			camMove->SetMoveTarget(*m_targetObj);
		}

		// 入ってきた面と出た面が同じなら 
		if (m_beforePoint != nullptr) {
			camMove->SetMoveTarget(*m_beforePoint);
		}

		//auto bp = m_beforePoint->GetComponent<CameraPointComponent>();
		//camMove->SetMoveTarget(bp->GetTargetPos());

		break;
	case CAMERA_MAX:
		break;
	default:
		break;
	}
}