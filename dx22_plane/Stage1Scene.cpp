#include "Stage1Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "Render3D.h"
#include "GameObjectManager.h"
#include "CircleMesh.h"
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
	camera->AddComponent<Camera>(*camera);
	camera->AddComponent<TransformComponent>(*camera);

	{
		CircleMesh circleMesh;
		auto circle = GameObjectManager::AddObject("circle0", "Circle");
		auto circleTrans = circle->AddComponent<TransformComponent>(*circle);
		circleTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
		auto circleRend = circle->AddComponent<Render3DComponent>(*circle);
		circleRend->SetMesh(circleMesh);
		circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		circleRend->SetTexture("assets/texture/NoTexture.png");
	}

	{
		CircleMesh circleMesh;
		auto circle = GameObjectManager::AddObject("circle1", "Circle");
		auto circleTrans = circle->AddComponent<TransformComponent>(*circle);
		circleTrans->SetPosition({ 10.0f, 0.0f, 0.0f });
		auto circleRend = circle->AddComponent<Render3DComponent>(*circle);
		circleRend->SetMesh(circleMesh);
		circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		circleRend->SetTexture("assets/texture/NoTexture.png");
	}

	{
		CircleMesh circleMesh;
		auto circle = GameObjectManager::AddObject("circle2", "Circle");
		auto circleTrans = circle->AddComponent<TransformComponent>(*circle);
		circleTrans->SetPosition({ -10.0f, 0.0f, 0.0f });
		auto circleRend = circle->AddComponent<Render3DComponent>(*circle);
		circleRend->SetMesh(circleMesh);
		circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		circleRend->SetTexture("assets/texture/NoTexture.png");
	}


}

//更新
void Stage1Scene::Update()
{

}

// 終了処理
void Stage1Scene::Uninit()
{
}

