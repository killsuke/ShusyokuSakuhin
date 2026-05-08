#include "CameraMoveComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "SpringComponent.h"
#include "Manager/GameObjectManager.h"
#include "CameraTargetComponent.h"
#include "RigidBodyComponent.h"
#include <iostream>

using namespace DirectX;

CameraMoveComponent::CameraMoveComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERA_MOVE"); // ソート番号を設定
	m_ChaseHeight = 25.0f; // 初期の追従高さは0
}

void CameraMoveComponent::Update()
{
	GameObject* cameraObj = GameObjectManager::GameObjectFindName("camera");
	GameObject* player = GameObjectManager::GameObjectFindName("Player");

    if(player == nullptr || cameraObj == nullptr) {
        return; // プレイヤーまたはカメラが見つからない場合は何もしない
	}

    TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
    
    if(trans == nullptr) {
        return; // TransformComponentが見つからない場合は何もしない
	}

    const XMFLOAT3 pos = trans->GetPosition();

    m_CameraPattern = CameraPattern::CAMERA_NONE;

    if (m_MoveTarget != nullptr) {

        m_CameraPattern = m_MoveTarget->GetComponent<CameraTargetComponent>()->GetCameraPattern();
    }

    if (m_CameraPattern == CameraPattern::CAMERA_NONE) {
        //  std::cout << "None" << std::endl;
    }
    else if (m_CameraPattern == CameraPattern::SPRING_CHASE) {
        //std::cout << "Spring" << std::endl;
    }

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
    SpringComponent* cameraSpring = cameraObj.GetComponent<SpringComponent>();
    if (cameraSpring != nullptr)
    {
        cameraSpring->SetActiveFlag(false);
    }

    CameraComponent* cameraComp = cameraObj.GetComponent<CameraComponent>();
    TransformComponent* cameraTrans = cameraObj.GetComponent<TransformComponent>();
    TransformComponent* playerTrans = player.GetComponent<TransformComponent>();

    if (cameraComp == nullptr || cameraTrans == nullptr || playerTrans == nullptr) {
        return; // CameraComponent、TransformComponentが見つからない場合は何もしない
    }

    const XMFLOAT3 playerPos = playerTrans->GetPosition();

    cameraTrans->SetPosition({ playerPos.x, cameraTrans->GetPosition().y, cameraTrans->GetPosition().z });
    cameraComp->SetTarget({ playerPos.x,cameraComp->GetTarget().y,cameraComp->GetTarget().z });
}

void CameraMoveComponent::ChaseXAndYCamera(GameObject& cameraObj, GameObject& player)
{
    CameraComponent* cameraComp = cameraObj.GetComponent<CameraComponent>();
    TransformComponent* cameraTrans = cameraObj.GetComponent<TransformComponent>();
    TransformComponent* playerTrans = player.GetComponent<TransformComponent>();

    if (cameraComp == nullptr || cameraTrans == nullptr || playerTrans == nullptr) {
        return; // CameraComponent、TransformComponentが見つからない場合は何もしない
    }

    const XMFLOAT3 playerPos = playerTrans->GetPosition();

        // ちょっとだけ上にするが、常に追従というよりは若干遅れるくらいにしたい、かつ、高さで追いかけるかどうかも決めたい
        // カメラ切替の当たり判定で切替もアリか
    cameraTrans->SetPosition({ playerPos.x, playerPos.y + 20.0f, cameraTrans->GetPosition().z });
    cameraComp->SetTarget({ playerPos.x,playerPos.y + 20.0f,cameraComp->GetTarget().z });
}

void CameraMoveComponent::Chase_XCamera(GameObject& cameraObj, GameObject& player) {

    SpringComponent* cameraSpring = cameraObj.GetComponent<SpringComponent>();
    if (cameraSpring != nullptr)
    {
        cameraSpring->SetActiveFlag(false);
    }

    CameraComponent* cameraComp = cameraObj.GetComponent<CameraComponent>();
    TransformComponent* cameraTrans = cameraObj.GetComponent<TransformComponent>();
    TransformComponent* playerTrans = player.GetComponent<TransformComponent>();

    if (cameraComp == nullptr || cameraTrans == nullptr || playerTrans == nullptr) {
        return; // CameraComponent、TransformComponentが見つからない場合は何もしない
    }

    const XMFLOAT3 playerPos = playerTrans->GetPosition();

    cameraTrans->SetPosition({ playerPos.x, cameraTrans->GetPosition().y, cameraTrans->GetPosition().z });
    cameraComp->SetTarget({ playerPos.x,cameraComp->GetTarget().y,cameraComp->GetTarget().z });
}

void CameraMoveComponent::Chase_YCamera(GameObject& cameraObj, GameObject& player) {

    TransformComponent* cameraTrans = cameraObj.GetComponent<TransformComponent>();
    CameraComponent* cameraComp = cameraObj.GetComponent<CameraComponent>();
    TransformComponent* playerTrans = player.GetComponent<TransformComponent>();

    if (cameraComp == nullptr || cameraTrans == nullptr || playerTrans == nullptr) {
        return; // CameraComponent、TransformComponentが見つからない場合は何もしない
    }

    const XMFLOAT3 playerPos = playerTrans->GetPosition();
    
    if (abs(playerPos.y) > m_ChaseHeight)
    {
        // ちょっと上ぐらいがちょうどいい
        cameraTrans->SetPosition({ cameraTrans->GetPosition().x, playerPos.y + 15.0f, cameraTrans->GetPosition().z });
        cameraComp->SetTarget({ cameraTrans->GetPosition().x,playerPos.y + 15.0f,cameraComp->GetTarget().z });
    }
}

void CameraMoveComponent::SpringCamera(GameObject& cameraObj)
{
    SpringComponent* cameraSpring = cameraObj.GetComponent<SpringComponent>();

    if (cameraSpring != nullptr)
    {
        // ここでバネが強制停止したかをチェック
        const bool fin = cameraSpring->GetFinSpringAction();
        if (fin == true) {
            cameraSpring->SetFinSpringAction(false);
            m_MoveTarget = nullptr;
            return;
        }

        // m_moveTargetから情報を全部もらうとか？
        cameraSpring->SetSpringPartner(m_MoveTarget);
        cameraSpring->SetK(m_MoveTarget->GetComponent<CameraTargetComponent>()->GetSpringK());
        cameraSpring->MakeDamping(); // ダンピングを作成
        cameraSpring->SpringActionTransform();
    }
}

void CameraMoveComponent::AdjustmentHeight(GameObject& cameraObj, GameObject& player)
{
    TransformComponent* cameraTrans = cameraObj.GetComponent<TransformComponent>();
    CameraComponent* cameraComp = cameraObj.GetComponent<CameraComponent>();
    TransformComponent* playerTrans = player.GetComponent<TransformComponent>();

    if(cameraComp == nullptr || cameraTrans == nullptr || playerTrans == nullptr) {
        return; // CameraComponent、TransformComponentが見つからない場合は何もしない
	}

    const XMFLOAT3 playerPos = playerTrans->GetPosition();
    
    if (abs(playerPos.y) > m_ChaseHeight)
    {
        // ちょっと上ぐらいがちょうどいい
        cameraTrans->SetPosition({ cameraTrans->GetPosition().x, playerPos.y + 15.0f, cameraTrans->GetPosition().z });
        cameraComp->SetTarget({ cameraTrans->GetPosition().x,playerPos.y + 15.0f,cameraComp->GetTarget().z });
    }
}

void CameraMoveComponent::SetMoveTarget(GameObject& target) {

    m_MoveTarget = &target;

    GameObject* cameraObj = GameObjectManager::GameObjectFindName("camera");

    if (cameraObj == nullptr) {
        return; // カメラオブジェクトが見つからない場合は何もしない
    }

    SpringComponent* cameraSpring = cameraObj->GetComponent<SpringComponent>();

    if (cameraSpring != nullptr)
    {
        cameraSpring->PreviousDistanceReset();
    }
}