#include "ResultScene.h"
#include "Components/CameraComponent.h"
#include "Manager/GameObjectManager.h"
#include "Components/TransformComponent.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/SphereMesh.h"
#include "Components/Render3DComponent.h"
#include "Components/Render2DComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/PlayerOperationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/JumpComponent.h"
#include "Input/Input.h"
#include "Manager/SceneManager.h"
#include "Components/DoorFadeComponent.h"
#include "Components/BlinkingUIComponent.h"

using namespace DirectX;

// コンストラクタ
ResultScene::ResultScene()
{
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(XMFLOAT3(10.0f, 30.0f, -170.0f));
	auto cameraObj = camera->AddComponent<CameraComponent>();
	cameraObj->SetTarget(XMFLOAT3(10.0f, 30.0f, 0.0f));
	auto fade = GameObjectManager::GameObjectFindTagUI("FadeUI");
	auto fadeUI = fade[0]->GetComponent<DoorFadeComponent>();

	auto ResultUI = GameObjectManager::AddUI("resultUI", "ResultUI");
	auto transTitle = ResultUI->AddComponent<TransformComponent>();
	transTitle->SetPosition({ 0.0f,30.0f,0.0f });
	transTitle->SetScale({ 700.0f,400.0f,1.0f });

	auto rendTitle = ResultUI->AddComponent<Render2DComponent>();
	rendTitle->CreateMesh<SquareMesh>();
	rendTitle->SetShader("ShaderResource/unlitTextureVS2D.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	if (fadeUI->GetWinLoseFlag() == true) {
		rendTitle->ChangeTexture("assets/texture/gameclear.png");
	}
	else {
		rendTitle->ChangeTexture("assets/texture/gameOver.png");
	}

	{
		auto titleUI = GameObjectManager::AddUI("tenmetuUI", "TitleUI");
		auto transTitle = titleUI->AddComponent<TransformComponent>();
		transTitle->SetPosition({ 0.0f,-200.0f,-0.5f });
		transTitle->SetScale({ 200.0f,100.0f,1.0f });
		auto blink = titleUI->AddComponent<BlinkingUIComponent>();
		blink->SetBlinkingSpeed(0.7f);

		auto rendTitle = titleUI->AddComponent<Render2DComponent>();
		rendTitle->CreateMesh<SquareMesh>();
		rendTitle->SetShader("ShaderResource/unlitTextureVS2D.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		rendTitle->ChangeTexture("assets/texture/please.png");
		rendTitle->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}



	if (!fade.empty()) {
		fadeUI->SetBootDoor(true);
	}
}

// デストラクタ
ResultScene::~ResultScene()
{
	GameObjectManager::OtherThanClear();
}

// 更新
void ResultScene::Update()
{

	if ((Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A)))
	{
		auto fade = GameObjectManager::GameObjectFindNameUI("fade");
		auto door = fade->GetComponent<DoorFadeComponent>();
		door->SetNextSceneName("TitleScene");

		door->SetBootDoor(true);
	}
}
