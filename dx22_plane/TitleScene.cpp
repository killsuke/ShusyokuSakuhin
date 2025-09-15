#include "TitleScene.h"
#include "Singleton.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "CubeMesh.h"
#include "SquareMesh.h"
#include "CircleMesh.h"
#include "SphereMesh.h"
#include "Render3D.h"
#include "Render2D.h"
#include "Render3DColliderAABBComponent.h"
#include "Collider.h"
#include "TestMoveComponent.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "Input.h"
#include "SceneManager.h"
#include "DoorFadeComponent.h"
#include "BlinkingUIComponent.h"

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
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 30.0f, -170.0f));
	auto cameraObj = camera->AddComponent<Camera>();
	cameraObj->SetTarget(DirectX::SimpleMath::Vector3(10.0f, 30.0f, 0.0f));

	{
		auto titleUI = GameObjectManager::AddUI("BackUI", "TitleUI");
		auto transTitle = titleUI->AddComponent<TransformComponent>();
		transTitle->SetPosition({ 0.0f,0.0f,1.0f });
		transTitle->SetScale({ 1080.0f,1000.0f,1.0f });
		SquareMesh square;
		auto rendTitle = titleUI->AddComponent<Render2DComponent>();
		rendTitle->SetMesh(square);
		rendTitle->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		rendTitle->SetTexture("assets/texture/NoTexture.png");
		rendTitle->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	}

	auto titleUI = GameObjectManager::AddUI("titleUI", "TitleUI");
	auto transTitle = titleUI->AddComponent<TransformComponent>();
	transTitle->SetPosition({ 0.0f,70.0f,0.0f });
	transTitle->SetScale({ 400.0f,280.0f,1.0f });
	SquareMesh square;
	auto rendTitle = titleUI->AddComponent<Render2DComponent>();
	rendTitle->SetMesh(square);
	rendTitle->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	rendTitle->SetTexture("assets/texture/Slash_Action_2.5D.png");

	{
		auto titleUI = GameObjectManager::AddUI("tenmetuUI", "TitleUI");
		auto transTitle = titleUI->AddComponent<TransformComponent>();
		transTitle->SetPosition({ 0.0f,-200.0f,-0.5f });
		transTitle->SetScale({ 200.0f,100.0f,1.0f });
		auto blink = titleUI->AddComponent<BlinkingUIComponent>();
		blink->SetBlinkingSpeed(0.7f);

		SquareMesh square;
		auto rendTitle = titleUI->AddComponent<Render2DComponent>();
		rendTitle->SetMesh(square);
		rendTitle->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		rendTitle->SetTexture("assets/texture/please.png");
		rendTitle->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	auto fadeFake = GameObjectManager::GameObjectFindNameUI("fade");
	if(fadeFake != nullptr) {
		auto fade =  fadeFake->GetComponent<DoorFadeComponent>();
		fade->SetBootDoor(true);
		fade->SetNextSceneName("LoadStageScene");

		//return;
	}

	auto fadeUI = GameObjectManager::AddUI("fade", "FadeUI");
	fadeUI->SetCarryOverFlag(true);
	auto fadeTrans = fadeUI->AddComponent<TransformComponent>();
	auto fadeAC = fadeUI->AddComponent<DoorFadeComponent>();
	fadeAC->SetNextSceneName("LoadStageScene");
}

// 更新
void TitleScene::Update()
{
	// エンターキーを押してステージ1へ
	if ((Input::GetKeyTrigger(VK_L) ))
	{
		auto fade = GameObjectManager::GameObjectFindNameUI("fade");
		auto door = fade->GetComponent<DoorFadeComponent>();

		door->SetBootDoor(true);
		//SceneManager::SceneChange<LoadStageScene>();
	}

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
