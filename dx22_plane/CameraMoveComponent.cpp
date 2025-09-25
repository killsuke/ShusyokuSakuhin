#include "CameraMoveComponent.h"
#include "Transform.h"
#include "Camera.h"
#include "Spring.h"
#include "GameObjectManager.h"
#include "CameraTargetComponent.h"
#include "RigidBodyComponent.h"
#include <iostream>
using namespace DirectX::SimpleMath;

CameraMoveComponent::CameraMoveComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("CAMERA_MOVE"); // ソート番号を設定
	m_chaseHeight = 25.0f; // 初期の追従高さは0
}

void CameraMoveComponent::Update()
{
	auto cameraObj = GameObjectManager::GameObjectFindName("camera");
	auto player = GameObjectManager::GameObjectFindName("Player");

	auto camerapos = cameraObj->GetComponent<RigidBodyComponent>()->GetVelocity();

//	std::cout << camerapos.x << "：" << camerapos.y << "：" << std::endl;

	//AdjustmentHeight(*cameraObj,*player); // プレイヤーの高さに合わせてカメラの高さを調整

	auto pos = p_object->GetComponent<TransformComponent>()->GetPosition();

	//std::cout << "CameraPos:" << pos.x << "," << pos.y << std::endl;

	CameraPattern nowCp = CAMERA_NONE;

	if (m_moveTarget != nullptr) {

		nowCp = m_moveTarget->GetComponent<CameraTargetComponent>()->GetCameraPattern();
	}

	switch (nowCp)
	{
	case CAMERA_NONE:
		// 何もしない
		break;
	case CHASE:
		ChaseCamera(*cameraObj, *player);
		break;
	case CHASE_X:
		Chase_XCamera(*cameraObj, *player);
		break;
	case CHASE_Y:
		Chase_YCamera(*cameraObj, *player);
		break;
	case SPRING_CHASE:
		SpringCamera(*cameraObj);
		break;
	case CAMERA_MAX:
		break;
	default:
		break;
	}
}

void CameraMoveComponent::ChaseCamera(GameObject& cameraObj, GameObject& player)
{
	auto cameraSpring = cameraObj.GetComponent<SpringComponent>();
	if (cameraSpring != nullptr)
	{
		cameraSpring->SetActiveFlag(false);
	}

	auto cameraComp = cameraObj.GetComponent<Camera>();
	auto cameraTrans = cameraObj.GetComponent<TransformComponent>();

	auto playerTrans = player.GetComponent<TransformComponent>();
	auto playerPos = playerTrans->GetPosition();

	cameraTrans->SetPosition({ playerPos.x, cameraTrans->GetPosition().y, cameraTrans->GetPosition().z });
//	cameraTrans->MakeWorldMatrix();
	cameraComp->SetTarget({ playerPos.x,cameraComp->GetTarget().y,cameraComp->GetTarget().z });
}

void CameraMoveComponent::Chase_XCamera(GameObject& cameraObj, GameObject& player) {
	auto cameraSpring = cameraObj.GetComponent<SpringComponent>();
	if (cameraSpring != nullptr)
	{
		cameraSpring->SetActiveFlag(false);
	}

	auto cameraComp = cameraObj.GetComponent<Camera>();
	auto cameraTrans = cameraObj.GetComponent<TransformComponent>();

	auto playerTrans = player.GetComponent<TransformComponent>();
	auto playerPos = playerTrans->GetPosition();

	cameraTrans->SetPosition({ playerPos.x, cameraTrans->GetPosition().y, cameraTrans->GetPosition().z });
//	cameraTrans->MakeWorldMatrix();
	cameraComp->SetTarget({ playerPos.x,cameraComp->GetTarget().y,cameraComp->GetTarget().z });
}

void CameraMoveComponent::Chase_YCamera(GameObject& cameraObj, GameObject& player) {
	auto cameraTrans = cameraObj.GetComponent<TransformComponent>();
	auto cameraComp = cameraObj.GetComponent<Camera>();

	auto playerTrans = player.GetComponent<TransformComponent>();
	auto playerPos = playerTrans->GetPosition();
	if (abs(playerPos.y) > m_chaseHeight)
	{
		// ちょっと上ぐらいがちょうどいい
		cameraTrans->SetPosition({ cameraTrans->GetPosition().x, playerPos.y + 15.0f, cameraTrans->GetPosition().z });
	//	cameraTrans->MakeWorldMatrix();
		cameraComp->SetTarget({ cameraTrans->GetPosition().x,playerPos.y + 15.0f,cameraComp->GetTarget().z });
	}
}

void CameraMoveComponent::SpringCamera(GameObject& cameraObj)
{
	auto cameraSpring = cameraObj.GetComponent<SpringComponent>();

	if (cameraSpring != nullptr)
	{
		// ここでバネが強制停止したかをチェック
		bool fin = cameraSpring->GetFinSpringAction();
		if (fin == true) {
			cameraSpring->SetFinSpringAction(false);
			m_moveTarget = nullptr;
			return;
		}

		// m_moveTargetから情報を全部もらうとか？
//		cameraSpring->SetActiveFlag(true);
		cameraSpring->SetSpringPartner(m_moveTarget);
		cameraSpring->SetK(m_moveTarget->GetComponent<CameraTargetComponent>()->GetSpringK());
		cameraSpring->MakeDamping(); // ダンピングを作成
		cameraSpring->SpringAction2D(); // 2Dのばね挙動を行う
	}
}

void CameraMoveComponent::AdjustmentHeight(GameObject& cameraObj, GameObject& player)
{
	auto cameraTrans = cameraObj.GetComponent<TransformComponent>();
	auto cameraComp = cameraObj.GetComponent<Camera>();

	auto playerTrans = player.GetComponent<TransformComponent>();
	auto playerPos = playerTrans->GetPosition();
	if (abs(playerPos.y) > m_chaseHeight)
	{
		// ちょっと上ぐらいがちょうどいい
		cameraTrans->SetPosition({ cameraTrans->GetPosition().x, playerPos.y + 15.0f, cameraTrans->GetPosition().z });
	//	cameraTrans->MakeWorldMatrix();
		cameraComp->SetTarget({ cameraTrans->GetPosition().x,playerPos.y + 15.0f,cameraComp->GetTarget().z });
	}
}