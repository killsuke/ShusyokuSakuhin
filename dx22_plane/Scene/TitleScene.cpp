#include "TitleScene.h"
#include "Components/Camera.h"
#include "Manager/GameObjectManager.h"
#include "Components/Transform.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/SphereMesh.h"
#include "Mesh/LineMesh.h"
#include "Components/Render3D.h"
#include "Components/Render2D.h"
#include "Components/RenderLine2DComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/Collider.h"
#include "Components/PlayerOperationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Input.h"
#include "Manager/SceneManager.h"
#include "Components/DoorFadeComponent.h"
#include "Components/TitleAnimationControlComponent.h"

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
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 30.0f, -900.0f));
	auto cameraObj = camera->AddComponent<Camera>();
	cameraObj->SetTarget(DirectX::SimpleMath::Vector3(10.0f, 30.0f, 0.0f));

	// 
	FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectXRender::SetClearColor(clearColor);

	GameObject* titleAnimation = GameObjectManager::AddObject("titleAnim", "TitleUI");
	TransformComponent* titleAnimTrans = titleAnimation->AddComponent<TransformComponent>();
	TitleAnimationControlComponent* titleAnimComp = titleAnimation->AddComponent<TitleAnimationControlComponent>();

	auto fadeFake = GameObjectManager::GameObjectFindNameUI("fade");
	if (fadeFake != nullptr) {
		auto fade = fadeFake->GetComponent<DoorFadeComponent>();
		fade->SetBootDoor(true);
		fade->SetNextSceneName("LoadStageScene");

		//return;
	}

	/*for (int num = 0; num < 10000; ++num) {
		GameObject* obj = GameObjectManager::AddObject("test","Test");
		TransformComponent* trans = obj->AddComponent<TransformComponent>();
		trans->SetScale(DirectX::XMFLOAT3(10.0f,10.0f,1.0f));
		Render3DComponent* rend = obj->AddComponent<Render3DComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	}*/

	auto fadeUI = GameObjectManager::AddUI("fade", "FadeUI");
	fadeUI->SetCarryOverFlag(true);
	auto fadeTrans = fadeUI->AddComponent<TransformComponent>();
	auto fadeAC = fadeUI->AddComponent<DoorFadeComponent>();
	fadeAC->SetNextSceneName("LoadStageScene");
}

// 更新
void TitleScene::Update()
{


	//else if ((Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A)) && setumeiFg == false)
	//{
	//	Texture2D* pt = Game::GetInstance()->AddObject<Texture2D>();
	//	pt->SetTexture("assets/texture/setumei.png");	// 画像を指定
	//	pt->SetPosition(0.0f, 0.0f, 0.0f);	// 位置を指定
	//	pt->SetRotation(0.0f, 0.0f, 0.0f);	// 角度を指定
	//	pt->SetScale(640.0f, 600.0f, 0.0f);// 大きさを指定
	//	m_MySceneObjects.emplace_back(pt);

	//	setumeiFg = true;
	//}




}

// 終了処理
void TitleScene::Uninit()
{
	//GameObjectManager::ListClear();	// ゲームオブジェクトマネージャーの終了処理
	GameObjectManager::OtherThanClear();
}
