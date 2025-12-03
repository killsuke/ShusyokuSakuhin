#include "Stage1Scene.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/Render3D.h"
#include "Components/Render2D.h"
#include "Components/RenderBillboard.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/LineMesh.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Components/HumanBoneComponent.h"
#include "Components/RenderLineComponent.h"
#include "Components/BonePartsComponent.h"
#include "Components/GoAroundComponent.h"
#include "Components/RenderBlurComponent.h"
#include "Components/RenderMotionBlurComponent.h"
#include "input.h"
#include "ModelManager.h"
#include "Components/Collider.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/TrailRenderComponent.h"

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

	//BonePartTest();

	//BoneToPartTest();

	//TestBlur();

	{
		//auto testObj = GameObjectManager::AddObject("testObj", "Test");
		//auto testTrans = testObj->AddComponent<TransformComponent>();
		//testTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
		//testTrans->SetScale({ 0.1f,0.1f,0.1f });
		//testTrans->SetRotation({ 0.0f,90.0f,0.0f });
		//auto collider = testObj->AddComponent<ColliderComponent>();
		//collider->SetOffsetSizeAABB({100.0f,100.0f,100.0f});
		//collider->SetOffsetCenterAABB({ 0.0f,0.0f,-50.0f });
		//auto collRend = testObj->AddComponent<Render3DColliderAABBComponent>();
		//auto rend = testObj->AddComponent<Render3DComponent>();
		//rend->LoadModelMesh("assets/model/Mountain/uploads_files_4307558_Mountain.fbx",
		//	"assets/model/Mountain");

		//rend->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
		////	rend->ChangeTexture("assets/texture/NoTexture.png");
		//rend->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	{
		//auto testObj = GameObjectManager::AddObject("testObj", "Test");
		//auto testTrans = testObj->AddComponent<TransformComponent>();
		//testTrans->SetPosition({ -100.0f, 0.0f, 150.0f });
		//testTrans->SetScale({ 0.1f,0.1f,0.1f });
		//testTrans->SetRotation({ 0.0f,90.0f,0.0f });
		//auto rend = testObj->AddComponent<Render3DComponent>();
		//rend->LoadModelMesh("assets/model/NewCar/uploads_files_3262252_r8.fbx",
		//	"assets/model/NewCar");

		//rend->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
		////	rend->ChangeTexture("assets/texture/NoTexture.png");
		//rend->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}

	{
		/*auto testObj = GameObjectManager::AddObject("testObj", "Test");
		auto testTrans = testObj->AddComponent<TransformComponent>();
		testTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
		testTrans->SetScale({ 10.0f,10.0f,10.0f });
		auto rend = testObj->AddComponent<TrailRenderComponent>();
		rend->SetBasePoint(-5.0f);
		rend->SetTipPoint(5.0f);*/
		//rend->CreateMesh<SquareMesh>();
//		rend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	rend->ChangeTexture("assets/texture/ranks.png");
//		rend->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	{
		/*	auto testObj = GameObjectManager::AddObject("testObj", "Test");
			auto testTrans = testObj->AddComponent<TransformComponent>();
			testTrans->SetPosition({ 400.0f, 0.0f, 0.0f });
			testTrans->SetScale({ 0.5f,0.5f,0.5f });
			auto rend = testObj->AddComponent<Render3DComponent>();
			rend->LoadModelMesh("assets/model/Container/12281_Container_v2_L2.obj",
				"assets/model/Container");

			rend->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
			rend->CreateMesh<SquareMesh>();
			rend->SetColor({ 1.0f,1.0f,1.0f,1.0f });*/
	}

	// モーションブラー用
	{
		auto testObj = GameObjectManager::AddObject("testObj", "Test");
		auto testTrans = testObj->AddComponent<TransformComponent>();
		testTrans->SetScale({ 30.0f,30.0f,30.0f });
		auto rend = testObj->AddComponent<RenderMotionBlurComponent>();
		rend->CreateMesh<CubeMesh>();
		rend->SetShader("shader/litTextureVS.hlsl", "shader/MotionBlurPS.hlsl");
		rend->ChangeTexture("assets/texture/dice.png");
		rend->SetBlurVelocity({ 0.5f,0.0f });

	}

	{
		auto testObj = GameObjectManager::AddObject("testObj", "Test");
		auto testTrans = testObj->AddComponent<TransformComponent>();
		testTrans->SetPosition({ 600.0f, 0.0f, 0.0f });
		testTrans->SetScale({ 0.5f,0.5f,0.5f });
		auto rend = testObj->AddComponent<Render3DComponent>();
		rend->LoadModelMesh("assets/model/Container/12281_Container_v2_L2.obj",
			"assets/model/Container");

		rend->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
		//	rend->ChangeTexture("assets/texture/NoTexture.png");
		rend->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
}

//更新
void Stage1Scene::Update()
{

}

// 終了処理
void Stage1Scene::Uninit()
{
	GameObjectManager::ListClear();	// ゲームオブジェクトマネージャーの終了処理
}

void Stage1Scene::BonePartTest()
{
	// 腰
	auto waist = GameObjectManager::AddObject("waist", "Part");
	auto waistTrans = waist->AddComponent<TransformComponent>();
	waistTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	waistTrans->SetScale({ 15.0f, 30.0f, 1.0f });
	auto waistbone = waist->AddComponent<BonePartsComponent>();
	waistbone->SetPartName("waist");
	waistbone->SetStartBonePosition({ 0.0f,0.0f,0.0f });
	waistbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	auto waistrend = waist->AddComponent<Render3DComponent>();

	waistrend->CreateMesh<SquareMesh>();
	waistrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	waistrend->ChangeTexture("assets/texture/NoTexture.png");
	waistrend->SetColor({ 0.0f,0.7f,0.0f,1.0f });

	// 胸
	auto chest = GameObjectManager::AddObject("chest", "Part");
	auto chestTrans = chest->AddComponent<TransformComponent>();
	chestTrans->SetLocalPosition({ 0.0f, 30.0f, 0.0f });
	chestTrans->SetScale({ 15.0f, 30.0f, 1.0f });
	auto chestbone = chest->AddComponent<BonePartsComponent>();
	chestbone->SetPartName("chest");
	chestbone->SetBoneParent(waistbone->GetAttachBone());
	chestbone->SetStartBonePosition({ 0.0f,-30.0f,0.0f });
	chestbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	chestbone->SetReferencePoint(waistbone->GetEndPoint());

	auto chestrend = chest->AddComponent<Render3DComponent>();
	chestrend->CreateMesh<SquareMesh>();
	chestrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	chestrend->ChangeTexture("assets/texture/NoTexture.png");
	chestrend->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	// 頭
	auto head = GameObjectManager::AddObject("head", "Part");
	auto headTrans = head->AddComponent<TransformComponent>();
	headTrans->SetLocalPosition({ 0.0f, 30.0f, 0.0f });
	headTrans->SetScale({ 15.0f, 30.0f, 1.0f });
	auto headbone = head->AddComponent<BonePartsComponent>();
	headbone->SetPartName("head");
	headbone->SetBoneParent(chestbone->GetAttachBone());
	headbone->SetStartBonePosition({ 0.0f,-30.0f,0.0f });
	headbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	headbone->SetReferencePoint(chestbone->GetEndPoint());

	auto headrend = head->AddComponent<Render3DComponent>();
	headrend->CreateMesh<SquareMesh>();
	headrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	headrend->ChangeTexture("assets/texture/NoTexture.png");
	headrend->SetColor({ 0.0f,0.0f,0.5f,1.0f });

}

void Stage1Scene::BoneToPartTest() {

	// ラインのテスト
	auto lineObj = GameObjectManager::AddObject("lineObj", "Line");
	auto lineTrans = lineObj->AddComponent<TransformComponent>();

	auto lineRender = lineObj->AddComponent<RenderLineComponent>();
	lineTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	lineRender->SetStartAndEndPosition({ 0.0f,0.0f,0.0f }, { 0.0f,100.0f,0.0f });
	lineRender->ChangeTexture("assets/texture/NoTexture.png");
	lineRender->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	lineRender->SetThickness(5.0f);
}

void Stage1Scene::TestBlur() {

	auto obj = GameObjectManager::AddObject("blurObj", "Blur");
	auto trans = obj->AddComponent<TransformComponent>();
	trans->SetPosition({ 0.0f, 0.0f, 0.0f });
	trans->SetScale({ 200.0f,200.0f,1.0f });
	auto rend = obj->AddComponent<RenderBlurComponent>();
	rend->CreateMesh<SquareMesh>();
	rend->SetShader("shader/unlitTextureVS.hlsl", "shader/BlurPS.hlsl");
	rend->ChangeTexture("assets/texture/setumei.png");
	rend->SetBlurTextureSize(Vector2(512.0f, 512.0f));
}