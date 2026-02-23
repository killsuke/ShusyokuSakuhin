#include "TitleScene.h"
#include "Components/CameraComponent.h"
#include "Manager/GameObjectManager.h"
#include "Components/TransformComponent.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/SphereMesh.h"
#include "Mesh/LineMesh.h"
#include "Components/Render3D.h"
#include "Components/Render2D.h"
#include "Components/RenderLine2DComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/PlayerOperationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Manager/SceneManager.h"
#include "Components/DoorFadeComponent.h"
#include "Components/TitleAnimationControlComponent.h"
#include "Components/TimeLineComponent.h"

// コンストラクタ
TitleScene::TitleScene()
{
	Init();
}

// デストラクタ
TitleScene::~TitleScene()
{
	Uninit();
}

// 初期化
void TitleScene::Init()
{
	GameObject* camera = GameObjectManager::AddObject("camera", "Camera");
	TransformComponent* cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 30.0f, -900.0f));
	CameraComponent* cameraObj = camera->AddComponent<CameraComponent>();
	cameraObj->SetTarget(DirectX::SimpleMath::Vector3(10.0f, 30.0f, 0.0f));

	FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectXRender::SetClearColor(clearColor);

	GameObject* titleAnimation = GameObjectManager::AddObject("titleAnim", "TitleUI");
	TransformComponent* titleAnimTrans = titleAnimation->AddComponent<TransformComponent>();
	TitleAnimationControlComponent* titleAnimComp = titleAnimation->AddComponent<TitleAnimationControlComponent>();

	GameObject* fadeFake = GameObjectManager::GameObjectFindNameUI("fade");
	if (fadeFake != nullptr) {
		DoorFadeComponent* fade = fadeFake->GetComponent<DoorFadeComponent>();
		fade->SetBootDoor(true);
		fade->SetNextSceneName("LoadStageScene");
	}
	else {
		GameObject* fadeUI = GameObjectManager::AddUI("fade", "FadeUI");
		fadeUI->SetCarryOverFlag(true);
		TransformComponent* fadeTrans = fadeUI->AddComponent<TransformComponent>();
		DoorFadeComponent* fadeAC = fadeUI->AddComponent<DoorFadeComponent>();
		fadeAC->SetNextSceneName("LoadStageScene");
	}
}

// 更新
void TitleScene::Update()
{
	//m_RecordTime += 0.016f; // 仮のデルタタイム

	//if(m_RecordTime > 10.0f) {
	//	GameObject* fadeFake = GameObjectManager::GameObjectFindNameUI("fade");
	//	if (fadeFake != nullptr) {
	//		DoorFadeComponent* fade = fadeFake->GetComponent<DoorFadeComponent>();
	//		fade->SetBootDoor(true);
	//		fade->SetNextSceneName("TitleScene");
	//	}
	//	SceneManager::SceneChange<TitleScene>();
	//}
}

// 終了処理
void TitleScene::Uninit()
{
	GameObjectManager::OtherThanClear();
}
