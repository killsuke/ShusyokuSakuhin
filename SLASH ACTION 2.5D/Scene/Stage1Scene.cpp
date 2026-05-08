#include "Stage1Scene.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Components/Render3DComponent.h"
#include "Components/Render2DComponent.h"
#include "Components/RenderBillboardComponent.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/LineMesh.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/TriangleMesh.h"
#include "Mesh/TriangularPrismMesh.h"
#include "Mesh/TerrainMesh.h"
#include "Components/RenderLineComponent.h"
#include "Components/RenderBlurComponent.h"
#include "Components/RenderMotionBlurCircularComponent.h"
#include "Input/Input.h"
#include "Manager/ModelManager.h"
#include "Components/ColliderComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/TrailRenderComponent.h"
#include "Components/ProjectileMotionComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/FliesMoveComponent.h"
#include "Components/MeshCut2DComponent.h"
#include "Components/VectorMoveComponent.h"
#include "Components/RenderTerrainComponent.h"
#include "Components/RenderTextureLuminescenceComponent.h"

using namespace DirectX;

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
	camera->AddComponent<CameraComponent>();
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

		//rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
		////	rend->ChangeTexture("NoTexture.png");
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

		//rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
		////	rend->ChangeTexture("NoTexture.png");
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
//		rend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	//	rend->ChangeTexture("ranks.png");
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

			rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
			rend->CreateMesh<SquareMesh>();
			rend->SetColor({ 1.0f,1.0f,1.0f,1.0f });*/
	}

	// モーションブラー用
	{
	/*	auto testObj = GameObjectManager::AddObject("testObj", "Test");
		auto testTrans = testObj->AddComponent<TransformComponent>();
		testTrans->SetScale({ 30.0f,30.0f,30.0f });
		auto rend = testObj->AddComponent<RenderMotionBlurComponent>();
		rend->CreateMesh<CubeMesh>();
		rend->SetShader("litTextureVS.hlsl", "MotionBlurPS.hlsl");
		rend->ChangeTexture("dice.png");
		rend->SetBlurVelocity({ 0.5f,0.0f });*/

	}

	// メッシュ接断用
	{
	/*	auto meshCutObj = GameObjectManager::AddObject("meshCutObj", "Test");
		auto meshCutTrans = meshCutObj->AddComponent<TransformComponent>();
		meshCutTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
		meshCutTrans->SetScale({ 70.0f,70.0f,1.0f });
		auto meshRend = meshCutObj->AddComponent<Render3DComponent>();
		meshRend->CreateMesh<SquareMesh>();
		meshRend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
		meshRend->SetColor({ 0.0f,0.0f,1.0f,1.0f });


		auto testLine = GameObjectManager::AddObject("line","Line");
		TransformComponent* trans = testLine->AddComponent<TransformComponent>();

		auto lineRender = testLine->AddComponent<RenderLineComponent>();
		trans->SetPosition({ 0.0f,0.0f,0.0f });
		lineRender->SetStartAndEndPosition({ 0.0f,70.0f,0.0f }, { 0.0f,-70.0f,0.0f });
		lineRender->ChangeTexture("NoTexture.png");
		lineRender->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		lineRender->SetThickness(3.0f);*/

	}


	//for (int num = 0; num < 10000; ++num) {
	//	GameObject* obj = GameObjectManager::AddObject("test","Test");
	//	TransformComponent* trans = obj->AddComponent<TransformComponent>();
	//	trans->SetScale(DirectX::XMFLOAT3(10.0f,10.0f,1.0f));
	//	Render3DComponent* rend = obj->AddComponent<Render3DComponent>();
	//	rend->CreateMesh<SquareMesh>();
	//	rend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	//}

	{
		auto testObj = GameObjectManager::AddObject("testObj", "Test");
		auto testTrans = testObj->AddComponent<TransformComponent>();
		testTrans->SetPosition({ 600.0f, 0.0f, 0.0f });
		testTrans->SetScale({ 100.5f,100.5f,150.5f });
		//testTrans->SetRotation({90.0f,0.0f,0.0f});
		auto rend = testObj->AddComponent<Render3DComponent>();
		rend->LoadModelMesh("assets/model/rock9/rock9_HIGH_RES.fbx",
			"assets/model/rock9");

		//rend->LoadModelMesh("assets/model/Container/12281_Container_v2_L2.obj",
		//	"assets/model/Container");

		rend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		//	rend->ChangeTexture("NoTexture.png");
		rend->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	{
		GameObject* parentObj = GameObjectManager::AddObject("parentObj", "Test");
		TransformComponent* parentTrans = parentObj->AddComponent<TransformComponent>();
		parentTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
		parentTrans->SetScale({ 50.0f,50.0f,50.0f });
		RenderTextureLuminescenceComponent* rend = parentObj->AddComponent<RenderTextureLuminescenceComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("dead_ring.png");
		rend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		rend->SetGlowRadius(5.0f);
		rend->SetExpand(1.5f);
		rend->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		rend->SetGlowColor({ 1.0f,0.0f,0.0f,1.0f });

		/*VectorMoveComponent* vectorMove = parentObj->AddComponent<VectorMoveComponent>();
		vectorMove->SetMoveDirection({ 1.0f,0.0f,0.0f });
		vectorMove->SetMovePower(2.0f);
		MeshCut2DComponent* meshCut = parentObj->AddComponent<MeshCut2DComponent>();
		meshCut->InitCutSettings(CutDirection::HORIZONTAL, 0.5f, 0.5f);*/
	//	RenderTerrainComponent* parentRend = parentObj->AddComponent<RenderTerrainComponent>();
		//parentRend->CreateMesh<TerrainMesh>();
	//	parentRend->SetShader("TerrainVS.hlsl", "TerrainPS.hlsl");
	//	parentRend->ChangeTexture("testTerrain.png");
	//	m_testID = parentObj->GetInstanceID();

		//GameObject* testObj = GameObjectManager::AddChild("testObj", "Test");
		//TransformComponent* testTrans = testObj->AddComponent<TransformComponent>();
		//testTrans->SetLocalPosition({ 50.0f, 0.0f, 0.0f });
		//testTrans->SetLocalScale({ 50.0f,50.0f,50.0f });
		///*VectorMoveComponent* vectorMove = testObj->AddComponent<VectorMoveComponent>();
		//vectorMove->SetMoveDirection({ 1.0f,0.0f,0.0f });
		//vectorMove->SetMovePower(5.0f);*/
		////RenderMotionBlurCircularComponent* rend = testObj->AddComponent<RenderMotionBlurCircularComponent>();
		//RenderMotionBlurCircularComponent* rend = testObj->AddComponent<RenderMotionBlurCircularComponent>();
		//rend->CreateMesh<SquareMesh>();
		//rend->ChangeTexture("background1.png");
		//rend->SetShader("PassThroughVS.hlsl", "unlitTexturePS.hlsl","MotionBlurCircularGS.hlsl");


		//parentObj->SetChild(testObj);
	}

	ID3D11Device* device = DirectXRender::GetDevice();
	ID3D11DeviceContext* context = DirectXRender::GetDeviceContext();

	/*auto manager = ::Effekseer::Manager::Create(8000);
	auto renderer = ::EffekseerRendererDX11::Renderer::Create(device, context, 8000);
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());*/
	//auto effect = Effekseer::Effect::Create(manager,L"C:\ProjectDanmakuGirls\ProjectDanmakuGirls");
}

//更新
void Stage1Scene::Update()
{
	GameObject* testObj = GameObjectManager::GameObjectFindInstanceIDAll(m_testID);
	if (testObj != nullptr) {
		TransformComponent* testTrans = testObj->GetComponent<TransformComponent>();
		testTrans->AddRotation({ 0.0f,0.0f,1.0f });
	}
}

// 終了処理
void Stage1Scene::Uninit()
{
	GameObjectManager::ListClear();	// ゲームオブジェクトマネージャーの終了処理
}

void Stage1Scene::BonePartTest()
{
	// 腰
	//auto waist = GameObjectManager::AddObject("waist", "Part");
	//auto waistTrans = waist->AddComponent<TransformComponent>();
	//waistTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	//waistTrans->SetScale({ 15.0f, 30.0f, 1.0f });
	//auto waistbone = waist->AddComponent<BonePartsComponent>();
	//waistbone->SetPartName("waist");
	//waistbone->SetStartBonePosition({ 0.0f,0.0f,0.0f });
	//waistbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	//auto waistrend = waist->AddComponent<Render3DComponent>();

	//waistrend->CreateMesh<SquareMesh>();
	//waistrend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	//waistrend->ChangeTexture("NoTexture.png");
	//waistrend->SetColor({ 0.0f,0.7f,0.0f,1.0f });

	//// 胸
	//auto chest = GameObjectManager::AddObject("chest", "Part");
	//auto chestTrans = chest->AddComponent<TransformComponent>();
	//chestTrans->SetLocalPosition({ 0.0f, 30.0f, 0.0f });
	//chestTrans->SetScale({ 15.0f, 30.0f, 1.0f });
	//auto chestbone = chest->AddComponent<BonePartsComponent>();
	//chestbone->SetPartName("chest");
	//chestbone->SetBoneParent(waistbone->GetAttachBone());
	//chestbone->SetStartBonePosition({ 0.0f,-30.0f,0.0f });
	//chestbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	//chestbone->SetReferencePoint(waistbone->GetEndPoint());

	//auto chestrend = chest->AddComponent<Render3DComponent>();
	//chestrend->CreateMesh<SquareMesh>();
	//chestrend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	//chestrend->ChangeTexture("NoTexture.png");
	//chestrend->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	//// 頭
	//auto head = GameObjectManager::AddObject("head", "Part");
	//auto headTrans = head->AddComponent<TransformComponent>();
	//headTrans->SetLocalPosition({ 0.0f, 30.0f, 0.0f });
	//headTrans->SetScale({ 15.0f, 30.0f, 1.0f });
	//auto headbone = head->AddComponent<BonePartsComponent>();
	//headbone->SetPartName("head");
	//headbone->SetBoneParent(chestbone->GetAttachBone());
	//headbone->SetStartBonePosition({ 0.0f,-30.0f,0.0f });
	//headbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	//headbone->SetReferencePoint(chestbone->GetEndPoint());

	//auto headrend = head->AddComponent<Render3DComponent>();
	//headrend->CreateMesh<SquareMesh>();
	//headrend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	//headrend->ChangeTexture("NoTexture.png");
	//headrend->SetColor({ 0.0f,0.0f,0.5f,1.0f });

}

void Stage1Scene::BoneToPartTest() {

	// ラインのテスト
	auto lineObj = GameObjectManager::AddObject("lineObj", "Line");
	auto lineTrans = lineObj->AddComponent<TransformComponent>();

	auto lineRender = lineObj->AddComponent<RenderLineComponent>();
	lineTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	lineRender->SetStartAndEndPosition({ 0.0f,0.0f,0.0f }, { 0.0f,100.0f,0.0f });
	lineRender->ChangeTexture("NoTexture.png");
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
	rend->SetShader("unlitTextureVS.hlsl", "BlurPS.hlsl");
	rend->ChangeTexture("setumei.png");
	rend->SetBlurTextureSize(XMFLOAT2(512.0f, 512.0f));
}