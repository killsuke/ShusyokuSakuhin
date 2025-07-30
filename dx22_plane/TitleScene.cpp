#include "TitleScene.h"
//#include "Texture2D.h"
#include "Singleton.h"
//#include "SkyDome.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "CubeMesh.h"
#include "SquareMesh.h"
#include "CircleMesh.h"
#include "SphereMesh.h"
#include "Render3D.h"
#include "Render3DColliderAABBComponent.h"
#include "Collider.h"
#include "TestMoveComponent.h"
#include "TestExtrusionComponent.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"

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
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -50.0f));
	camera->AddComponent<Camera>();

	{
		auto cube = GameObjectManager::AddObject("cube1", "Cube");
		cube->AddComponent<TestMoveComponent>();

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 10.0f, 10.0f, 10.0f });
		cubeTrans->SetPosition({ 30.0f,30.0f,0.0f });

		auto cubeJump = cube->AddComponent<JumpComponent>();
		cubeJump->SetJumpPower(50.0f);

		auto cubeRigid = cube->AddComponent<RigidBodyComponent>();
		cubeRigid->SetMass(2.0f);

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto cubeCollEX = cube->AddComponent<TestExtrusionComponent>();

		CubeMesh cubeMesh;	// 四角形のメッシュ
		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	}

	{
		auto cube = GameObjectManager::AddObject("cube2", "Cube");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 100.0f, 20.0f, 10.0f });
		cubeTrans->SetPosition({ 0.0f, -20.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		CubeMesh cubeMesh;
		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));
	}

	{
		auto cube = GameObjectManager::AddObject("cube3", "Cube");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 20.0f, 30.0f, 10.0f });
		cubeTrans->SetPosition({ 0.0f, 40.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		CubeMesh cubeMesh;
		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));
	}

}

// 更新
void TitleScene::Update()
{
	// エンターキーを押してステージ1へ
	//if ((Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A)) && setumeiFg == true)
	//{
	//	Game::GetInstance()->ChangeScene(STAGE1);
	//}

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

}
