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
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	camera->AddComponent<Camera>();
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition({ 0.0f, 0.0f, -500.0f });

	/*auto inchWorm = GameObjectManager::AddObject("inch", "InchWorm");
	auto inchTrans = inchWorm->AddComponent<TransformComponent>();
	inchTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	inchTrans->SetScale({ 50.0f, 50.0f, 1.0f });
	inchWorm->AddComponent<HumanBoneComponent>();*/




	//// 腰
	//{
	//	CircleMesh circleMesh;
	//	auto circle = GameObjectManager::AddAbsFront("joint0", "Circle");
	//	auto circleTrans = circle->AddComponent<TransformComponent>();
	//	circleTrans->SetPosition({ 0.0f, -80.0f, 10.0f });
	//	circleTrans->SetScale({10.0f,10.0f,10.0f});
	//	auto circleRend = circle->AddComponent<RenderBillboardComponent>();
	//	circleRend->SetMesh(circleMesh);
	//	circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	circleRend->SetTexture("assets/texture/NoTexture.png");
	//}

	// デバッグ用のボーン
	//auto boneLine = GameObjectManager::AddAbsFront("boneLine", "Line");
	//auto boneLineTrans = boneLine->AddComponent<TransformComponent>();
	//boneLineTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	//boneLineTrans->SetScale({ 10.0f, 10.0f, 1.0f });
	//auto boneLineRend = boneLine->AddComponent<RenderLineComponent>();
	////	circleRend->SetThickness(30.0f);
	//boneLineRend->SetTexture("assets/texture/NoTexture.png");
	//boneLineRend->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	//auto boneLine2 = GameObjectManager::AddAbsFront("boneLine2", "Line");
	//auto boneLineTrans2 = boneLine2->AddComponent<TransformComponent>();
	//boneLineTrans2->SetPosition({ 0.0f, 0.0f, 0.0f });
	//boneLineTrans2->SetScale({ 10.0f, 10.0f, 1.0f });
	//auto boneLineRend2 = boneLine2->AddComponent<RenderLineComponent>();
	////	circleRend->SetThickness(30.0f);
	//boneLineRend2->SetTexture("assets/texture/NoTexture.png");
	//boneLineRend2->SetColor({ 1.0f,0.0f,0.0f,1.0f });

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
	SquareMesh waistsquare;
	waistrend->SetMesh(waistsquare);
	waistrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	waistrend->SetTexture("assets/texture/NoTexture.png");
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
	SquareMesh chestsquare;
	chestrend->SetMesh(chestsquare);
	chestrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	chestrend->SetTexture("assets/texture/NoTexture.png");
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
	SquareMesh headsquare;
	headrend->SetMesh(headsquare);
	headrend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	headrend->SetTexture("assets/texture/NoTexture.png");
	headrend->SetColor({ 0.0f,0.0f,0.5f,1.0f });

	//boneLineRend->SetStartAndEndFollowObject(waist, chest);
	//boneLineRend2->SetStartAndEndFollowObject(chest, head);

	//auto rolling = GameObjectManager::AddObject("rolling", "Sword");

	//auto rollingTrans = rolling->AddComponent<TransformComponent>();
	//rollingTrans->SetScale({ 20.0f, 20.0f, 3.0f });
	//rollingTrans->SetPosition({ 30.0f,-9.0f,3.0f });
	//rollingTrans->SetRotation({ 0.0f, 0.0f, 0.0f });

	//auto rollingGoAround = rolling->AddComponent<GoAroundComponent>();
	//rollingGoAround->SetCenterObject(waist); // プレイヤーを中心に回るように設定
	//rollingGoAround->MakeInitialOffset(waistTrans->GetPosition(), rollingTrans->GetPosition()); // 初期オフセットを設定
	////rollingGoAround->SetInitialAngle(0.0f);
	//rollingGoAround->SetRotationSpeed(20.0f); // 回転速度を設定
	//rollingGoAround->SetRollingActive(true);
	//rollingGoAround->SetClockwise(true);

	//SquareMesh rollingMesh;
	//auto rollingRender = rolling->AddComponent<Render3DComponent>();
	//rollingRender->SetMesh(rollingMesh);
	//rollingRender->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//rollingRender->SetTexture("assets/texture/sword.png");

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
	GameObjectManager::ListClear();	// ゲームオブジェクトマネージャーの終了処理
}

