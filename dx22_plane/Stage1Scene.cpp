#include "Stage1Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "Render3D.h"
#include "GameObjectManager.h"
#include "CircleMesh.h"
#include "CubeMesh.h"
#include "SquareMesh.h"
#include "InchWorm_Test.h"

using namespace DirectX::SimpleMath;

// コンストラクタ
Stage1Scene::Stage1Scene()
{
	Init();
}

// デストラクタ
Stage1Scene::~Stage1Scene()
{
	Uninit();
}

// 初期化
void Stage1Scene::Init()
{
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	camera->AddComponent<Camera>();
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition({ 0.0f, 0.0f, -500.0f });

	auto inchWorm = GameObjectManager::AddObject("inch", "InchWorm");
	auto inchTrans = inchWorm->AddComponent<TransformComponent>();
	inchTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	inchTrans->SetScale({ 50.0f, 50.0f, 1.0f });
	inchWorm->AddComponent<InchWorm_Test>();


	// 腰
	{
		CubeMesh circleMesh;
		auto circle = GameObjectManager::AddObject("joint0", "Circle");
		auto circleTrans = circle->AddComponent<TransformComponent>();
		circleTrans->SetPosition({ 0.0f, -10.0f, -10.0f });
		circleTrans->SetScale({10.0f,10.0f,10.0f});
//		circleTrans->SetRotation({0.0f,60.0f,0.0f});
		auto circleRend = circle->AddComponent<Render3DComponent>();
		circleRend->SetMesh(circleMesh);
		circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		circleRend->SetTexture("assets/texture/NoTexture.png");
	}

	//// 胸
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint1", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 頭
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint2", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 0.0f, 15.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 左肩
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint3", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 10.0f, 5.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 左ひじ
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint4", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 20.0f, 5.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 左手
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint5", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 30.0f, 5.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 右肩
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint6", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ -10.0f, 5.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 右ひじ
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint7", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ -20.0f, 5.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 右手
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint8", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ -30.0f, 5.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 左足付け根
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint9", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 5.0f, -15.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 左膝
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint10", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 5.0f, -25.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 左足首
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint11", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 5.0f, -35.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 右足付け根
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint12", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ -5.0f, -15.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 右膝
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint13", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ -5.0f, -25.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	//// 右足首
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddObject("joint14", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ -5.0f, -35.0f, 0.0f });
	//	auto circleRend = circle->AddComponent<Render3DComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}




}

//更新
void Stage1Scene::Update()
{

}

// 終了処理
void Stage1Scene::Uninit()
{
}

