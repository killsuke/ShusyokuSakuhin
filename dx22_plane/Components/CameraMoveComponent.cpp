#include "CameraMoveComponent.h"
#include "Transform.h"
#include "Camera.h"
#include "SpringComponent.h"
#include "GameObjectManager.h"
#include "CameraTargetComponent.h"
#include "RigidBodyComponent.h"
#include <iostream>
using namespace DirectX::SimpleMath;

CameraMoveComponent::CameraMoveComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERA_MOVE"); // ソート番号を設定
	m_chaseHeight = 25.0f; // 初期の追従高さは0
}

void CameraMoveComponent::Update()
{
	auto cameraObj = GameObjectManager::GameObjectFindName("camera");
	auto player = GameObjectManager::GameObjectFindName("Player");

	auto camerapos = cameraObj->GetComponent<RigidBodyComponent>()->GetVelocity();

//	std::cout << camerapos.x << "：" << camerapos.y << "：" << std::endl;

	//AdjustmentHeight(*cameraObj,*player); // プレイヤーの高さに合わせてカメラの高さを調整

	auto pos = m_Object->GetComponent<TransformComponent>()->GetPosition();

	//std::cout << "CameraPos:" << pos.x << "," << pos.y << std::endl;

	m_CameraPattern = CameraPattern::CAMERA_NONE;

	if (m_moveTarget != nullptr) {

		m_CameraPattern = m_moveTarget->GetComponent<CameraTargetComponent>()->GetCameraPattern();
	}

	if (m_CameraPattern == CameraPattern::CAMERA_NONE) {
	//	std::cout << "None" << std::endl;
	}
	else if (m_CameraPattern == CameraPattern::SPRING_CHASE) {
		//std::cout << "Spring" << std::endl;
	}

	// 一旦プレイヤー追従にしておく
	// ブロックは一度動かしてみて、
	// バッタがちゃんと目的地に移動できるか試す
//	nowCp = CameraPattern::CHASE_XANDY;

	switch (m_CameraPattern)
	{
	case CameraPattern::CAMERA_NONE:
		// 何もしない
		break;
	case CameraPattern::CHASE:
		ChaseCamera(*cameraObj, *player);
		break;
	case CameraPattern::CHASE_XANDY:
		ChaseXAndYCamera(*cameraObj, *player);
		break;
	case CameraPattern::CHASE_X:
		Chase_XCamera(*cameraObj, *player);
		break;
	case CameraPattern::CHASE_Y:
		Chase_YCamera(*cameraObj, *player);
		break;
	case CameraPattern::SPRING_CHASE:
		SpringCamera(*cameraObj);
		break;
	case CameraPattern::CAMERA_MAX:
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
	cameraComp->SetTarget({ playerPos.x,cameraComp->GetTarget().y,cameraComp->GetTarget().z });
}

void CameraMoveComponent::ChaseXAndYCamera(GameObject& cameraObj, GameObject& player)
{
	auto cameraComp = cameraObj.GetComponent<Camera>();
	auto cameraTrans = cameraObj.GetComponent<TransformComponent>();

	auto playerTrans = player.GetComponent<TransformComponent>();
	auto playerPos = playerTrans->GetPosition();

//	cameraTrans->SetRotation({ 60.0f,0.0f,0.0f });
	cameraTrans->SetPosition({ playerPos.x, playerPos.y, cameraTrans->GetPosition().z });
	cameraComp->SetTarget({ playerPos.x,playerPos.y,cameraComp->GetTarget().z });
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
		//cameraSpring->SpringAction2D(); // 2Dのばね挙動を行う

		cameraSpring->SpringActionTransform();
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

void CameraMoveComponent::SetMoveTarget(GameObject& target) {
	m_moveTarget = &target; 
	
	auto cameraObj = GameObjectManager::GameObjectFindName("camera");
	auto cameraSpring = cameraObj->GetComponent<SpringComponent>();

	if (cameraSpring != nullptr)
	{
		cameraSpring->PreviousDistanceReset();
	}
}
