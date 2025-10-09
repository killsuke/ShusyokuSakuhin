#include "Stage1Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "Render3D.h"
#include "Render2D.h"
#include "RenderBillboard.h"
#include "GameObjectManager.h"
#include "CircleMesh.h"
#include "LineMesh.h"
#include "CubeMesh.h"
#include "SquareMesh.h"
#include "HumanBoneComponent.h"
#include "RenderLineComponent.h"
#include "BonePartsComponent.h"
#include "GoAroundComponent.h"
#include "input.h"

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
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();

	auto camera = gameObjectManager.AddObject("camera", "Camera");
	camera->AddComponent<Camera>();
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition({ 0.0f, 0.0f, -500.0f });

	// 腰
	auto waist = gameObjectManager.AddObject("waist", "Part");
	auto waistTrans = waist->AddComponent<TransformComponent>();
	waistTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	waistTrans->SetScale({ 15.0f, 30.0f, 1.0f });
	auto waistbone = waist->AddComponent<BonePartsComponent>();
	waistbone->SetPartName("waist");
	waistbone->SetStartBonePosition({ 0.0f,0.0f,0.0f });
	waistbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	auto waistrend = waist->AddComponent<Render3DComponent>();
	SquareMesh waistsquare;
	waistrend->SetMesh(waistsquare);
	waistrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	waistrend->SetTexture("assets/texture/NoTexture.png");
	waistrend->SetColor({ 0.0f,0.7f,0.0f,1.0f });

	// 胸
	auto chest = gameObjectManager.AddObject("chest", "Part");
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
	SquareMesh chestsquare;
	chestrend->SetMesh(chestsquare);
	chestrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	chestrend->SetTexture("assets/texture/NoTexture.png");
	chestrend->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	// 頭
	auto head = gameObjectManager.AddObject("head", "Part");
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
	SquareMesh headsquare;
	headrend->SetMesh(headsquare);
	headrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	headrend->SetTexture("assets/texture/NoTexture.png");
	headrend->SetColor({ 0.0f,0.0f,0.5f,1.0f });

	// 左肩
	auto leftShoulder = gameObjectManager.AddObject("leftShoulder", "Part");
	auto leftShoulderTrans = leftShoulder->AddComponent<TransformComponent>();
	leftShoulderTrans->SetLocalPosition({ 15.0f, 2.0f, 0.0f });
	leftShoulderTrans->SetLocalRotation({ 0.0f, 0.0f, -45.0f });
	leftShoulderTrans->SetScale({ 10.0f, 10.0f, 1.0f });
	auto leftShoulderbone = leftShoulder->AddComponent<BonePartsComponent>();
	leftShoulderbone->SetPartName("leftShoulder");
	leftShoulderbone->SetBoneParent(chestbone->GetAttachBone());
	leftShoulderbone->SetStartBonePosition({ 0.0f,-10.0f,0.0f });
	leftShoulderbone->SetEndBonePosition({ 0.0f,10.0f,0.0f });
	leftShoulderbone->SetReferencePoint(chestbone->GetEndPoint());

	auto leftShoulderrend = leftShoulder->AddComponent<Render3DComponent>();
	SquareMesh leftShouldersquare;
	leftShoulderrend->SetMesh(leftShouldersquare);
	leftShoulderrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	leftShoulderrend->SetTexture("assets/texture/NoTexture.png");
	leftShoulderrend->SetColor({ 0.5f,0.0f,0.0f,1.0f });

	// 左上腕
	auto upperLeftArm = gameObjectManager.AddObject("upperLeftArm", "Part");
	auto upperLeftArmTrans = upperLeftArm->AddComponent<TransformComponent>();
	upperLeftArmTrans->SetLocalPosition({ 20.0f, 20.0f, 0.0f });
	upperLeftArmTrans->SetLocalRotation({ 0.0f, 0.0f, -45.0f });
	upperLeftArmTrans->SetScale({ 10.0f, 30.0f, 1.0f });
	auto upperLeftArmbone = upperLeftArm->AddComponent<BonePartsComponent>();
	upperLeftArmbone->SetPartName("upperLeftArm");
	upperLeftArmbone->SetBoneParent(leftShoulderbone->GetAttachBone());
	upperLeftArmbone->SetStartBonePosition({ 0.0f,-30.0f,0.0f });
	upperLeftArmbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	upperLeftArmbone->SetReferencePoint(leftShoulderbone->GetEndPoint());
	auto upperLeftArmrend = upperLeftArm->AddComponent<Render3DComponent>();
	SquareMesh upperLeftArmsquare;
	upperLeftArmrend->SetMesh(upperLeftArmsquare);
	upperLeftArmrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	upperLeftArmrend->SetTexture("assets/texture/NoTexture.png");
	upperLeftArmrend->SetColor({ 0.5f,0.5f,0.0f,1.0f });

	// 左前腕
	auto lowerLeftArm = gameObjectManager.AddObject("lowerLeftArm", "Part");
	auto lowerLeftArmTrans = lowerLeftArm->AddComponent<TransformComponent>();
	lowerLeftArmTrans->SetLocalPosition({ 0.0f, 30.0f, 0.0f });
	lowerLeftArmTrans->SetLocalRotation({ 0.0f, 0.0f, 0.0f });
	lowerLeftArmTrans->SetScale({ 10.0f, 30.0f, 1.0f });
	auto lowerLeftArmbone = lowerLeftArm->AddComponent<BonePartsComponent>();
	lowerLeftArmbone->SetPartName("lowerLeftArm");
	lowerLeftArmbone->SetBoneParent(upperLeftArmbone->GetAttachBone());
	lowerLeftArmbone->SetStartBonePosition({ 0.0f,-30.0f,0.0f });
	lowerLeftArmbone->SetEndBonePosition({ 0.0f,30.0f,0.0f });
	lowerLeftArmbone->SetReferencePoint(upperLeftArmbone->GetEndPoint());
	auto lowerLeftArmrend = lowerLeftArm->AddComponent<Render3DComponent>();
	SquareMesh lowerLeftArmsquare;
	lowerLeftArmrend->SetMesh(lowerLeftArmsquare);
	lowerLeftArmrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	lowerLeftArmrend->SetTexture("assets/texture/NoTexture.png");
	lowerLeftArmrend->SetColor({ 0.5f,0.0f,0.5f,1.0f });


	
}

//更新
void Stage1Scene::Update()
{

}

// 終了処理
void Stage1Scene::Uninit()
{
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();

	gameObjectManager.ListClear();	// ゲームオブジェクトマネージャーの終了処理
}

