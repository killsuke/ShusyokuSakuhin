#include "ResultScene.h"
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
ResultScene::ResultScene()
{
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();

	auto camera = gameObjectManager.AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 30.0f, -170.0f));
	auto cameraObj = camera->AddComponent<Camera>();
	cameraObj->SetTarget(DirectX::SimpleMath::Vector3(10.0f, 30.0f, 0.0f));
	auto fade = gameObjectManager.GameObjectFindTagUI("FadeUI");
	auto fadeUI = fade[0]->GetComponent<DoorFadeComponent>();

	/*{
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
	}*/

	auto ResultUI = gameObjectManager.AddUI("resultUI", "ResultUI");
	auto transTitle = ResultUI->AddComponent<TransformComponent>();
	transTitle->SetPosition({ 0.0f,30.0f,0.0f });
	transTitle->SetScale({ 700.0f,400.0f,1.0f });
	SquareMesh square;
	auto rendTitle = ResultUI->AddComponent<Render2DComponent>();
	rendTitle->SetMesh(square);
	rendTitle->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	if (fadeUI->GetWinLoseFlag() == true) {
		rendTitle->SetTexture("assets/texture/gameclear.png");
	}
	else{
		rendTitle->SetTexture("assets/texture/gameOver.png");
	}

	{
		auto titleUI = gameObjectManager.AddUI("tenmetuUI", "TitleUI");
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



	if (!fade.empty()) {
		fadeUI->SetBootDoor(true);
	}
}

// デストラクタ
ResultScene::~ResultScene()
{
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager.OtherThanClear();
}

// 更新
void ResultScene::Update()
{
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();

	if ((Input::GetKeyTrigger(VK_L) || Input::GetButtonTrigger(XINPUT_X)))
	{
		auto fade = gameObjectManager.GameObjectFindNameUI("fade");
		auto door = fade->GetComponent<DoorFadeComponent>();
		door->SetNextSceneName("TitleScene");

		door->SetBootDoor(true);
	}
}
