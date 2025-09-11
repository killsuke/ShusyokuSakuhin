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

	auto titleUI = GameObjectManager::AddUI("titleUI", "TitleUI");
	auto transTitle = titleUI->AddComponent<TransformComponent>();
	transTitle->SetPosition({ 0.0f,0.0f,0.0f });
	transTitle->SetScale({ 480.0f,320.0f,1.0f });
	SquareMesh square;
	auto rendTitle = titleUI->AddComponent<Render2DComponent>();
	rendTitle->SetMesh(square);
	rendTitle->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	rendTitle->SetTexture("assets/texture/title_car.png");

	auto fadeUI = GameObjectManager::AddUI("fade", "FadeUI");
	auto fadeTrans = fadeUI->AddComponent<TransformComponent>();
	auto fadeAC = fadeUI->AddComponent<DoorFadeComponent>();
}

// 更新
void TitleScene::Update()
{
	// エンターキーを押してステージ1へ
	if ((Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A)))
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
	GameObjectManager::OtherThanTagClear("FadeUI");
}
