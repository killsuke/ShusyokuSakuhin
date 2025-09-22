#include "LoadStageScene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "CubeMesh.h"
#include "SquareMesh.h"
#include "CircleMesh.h"
#include "SphereMesh.h"
#include "Render3D.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "Render2D.h"
#include "Collider.h"
#include "TestMoveComponent.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "EnemyDamageComponent.h"
#include "FighterComponent.h"
#include "AttackTimingComponent.h"
#include "AttackOneTimeComponent.h"
#include "CameraMoveComponent.h"
#include "CameraPointComponent.h"
#include "CameraTargetComponent.h"
#include "GoAroundComponent.h"
#include "HPBarMoveComponent.h"
#include "Spring.h"
#include "StageLoadCSVComponent.h"
#include "TerrainManagerComponent.h"
#include "EnemyManagerComponent.h"
#include "TerrainJsonComponent.h"
#include "EnemyJsonComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "SkyDomeMesh.h"
#include "SkyDomeRenderComponent.h"
#include "TestSwordActionComponent.h"
#include "DoorFadeComponent.h"
#include "BossEventComponent.h"

LoadStageScene::LoadStageScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 30.0f, -170.0f));
	camera->AddComponent<RigidBodyComponent>();
	auto cameraMove = camera->AddComponent<CameraMoveComponent>();
	auto cameraComp = camera->AddComponent<Camera>();
	cameraComp->SetTarget(DirectX::SimpleMath::Vector3(10.0f, 30.0f, 0.0f));

	camera->AddComponent<SpringComponent>();

	{
		auto skydome = GameObjectManager::AddObject("skyDome", "SkyDome");
		auto skyTrans = skydome->AddComponent<TransformComponent>();
		skyTrans->SetScale({ 1000.0f,500.0f,1000.0f });
		auto skyRend = skydome->AddComponent<SkyDomeRenderComponent>();
		SkyDomeMesh skyMesh;
		skyRend->SetMesh(skyMesh);
		skyRend->SetShader("unlitTextureVSSkyDome.hlsl", "shader/unlitTexturePS.hlsl");
		skyRend->TextureLoadSkyDome(L"assets/texture/skyDome.dds");
	}

	{
		auto stageRoadCSV = GameObjectManager::AddObject("StageRoadCSV", "StageRoadCSV");
		auto stageRoadCSVTrans = stageRoadCSV->AddComponent<TransformComponent>();
		auto str = stageRoadCSV->AddComponent<StageLoadCSVComponent>();
		auto teM = stageRoadCSV->AddComponent<TerrainManagerComponent>();
		auto enM = stageRoadCSV->AddComponent<EnemyManagerComponent>();

		auto terrainJson = stageRoadCSV->AddComponent<TerrainJsonComponent>();
		terrainJson->LoadTerrainJsonFile("json/terrain.json");
		auto terrainStatus = terrainJson->GetTerrainStatus();
		//	auto terrainKinds = terrainJson->GetKindNames();

		auto enemyJson = stageRoadCSV->AddComponent<EnemyJsonComponent>();
		enemyJson->LoadEnemyJsonFile("json/enemy.json");
		auto enemyStatus = enemyJson->GetEnemyStatus();

		//	enemyJson->MakeSampleStatus(); // サンプルの敵キャラ情報を作成

		str->LoadStageCSV("Stage1.csv", *stageRoadCSV); // ステージのCSVを読み込む
		teM->CreateTerrains(terrainStatus); // 読み込んだCSVからTerrainを生成
		enM->CreateEnemies(enemyStatus); // 読み込んだJSONからEnemyを生成
	}

	GameObject* playOBJ = nullptr;
	{
		auto player = GameObjectManager::AddObject("Player", "Player");
		playOBJ = player;
		
		player->AddComponent<TestMoveComponent>();

		auto playerTrans = player->AddComponent<TransformComponent>();
		playerTrans->SetScale({ 6.0f, 10.0f, 5.0f });
		playerTrans->SetPosition({ 0.0f,-9.0f,0.0f });
		playerTrans->SetRotation({ 0.0f, 0.0f, 0.0f });

		auto cubeJump = player->AddComponent<JumpComponent>();
		cubeJump->SetJumpPower(50.0f);

		auto cubeRigid = player->AddComponent<RigidBodyComponent>();
		cubeRigid->SetMass(2.0f);
		cubeRigid->SetGravityFlag(true);

		player->AddComponent<TestExtrusionJudgeComponent>();

		auto cubeColl = player->AddComponent<ColliderComponent>();
		//	cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
			//		cubeColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(20.0f, 5.0f, 0.0f));
			//		cubeColl->SetOffsetRotationOBB(DirectX::XMFLOAT3(0.0f, 0.0f, 45.0f));

		auto fighterPlayer = player->AddComponent<FighterComponent>();
		fighterPlayer->SetHp(50);
		fighterPlayer->SetMaxHp(50);
		fighterPlayer->SetAtk(10);
		fighterPlayer->SetUseInvincible(true);
		fighterPlayer->SetUseDeadFlag(false);

		SquareMesh cubeMesh;	// 四角形のメッシュ
		auto cubeRe = player->AddComponent<Render2DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("Animation2DVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/aka.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = player->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));


		//	auto rolling = GameObjectManager::AddObject("rolling", "Sword");
		auto rolling = GameObjectManager::AddObject("rolling", "Sword");

		auto rollingTrans = rolling->AddComponent<TransformComponent>();
		rollingTrans->SetScale({ 11.0f, 4.0f, 3.0f });
		rollingTrans->SetPosition({ 30.0f,-9.0f,3.0f });
		rollingTrans->SetRotation({ 0.0f, 0.0f, 0.0f });

		auto rollingColl = rolling->AddComponent<ColliderComponent>();
		rollingColl->SetOffsetSizeOBB({ -3.0f,0.0f,3.0f });
		rollingColl->SetOffsetCenterOBB({ -3.0f,0.0f,0.0f });

		auto testAction = rolling->AddComponent<TestSwordActionComponent>();
		testAction->SetHolder(player);

		auto rollingGoAround = rolling->AddComponent<GoAroundComponent>();
		rollingGoAround->SetCenterObject(player); // プレイヤーを中心に回るように設定
		rollingGoAround->MakeInitialOffset(playerTrans->GetPosition(), rollingTrans->GetPosition()); // 初期オフセットを設定
		rollingGoAround->SetInitialAngle(90.0f);
		rollingGoAround->SetRotationSpeed(7.0f); // 回転速度を設定
		rollingGoAround->SetRollingActive(false);
		rollingGoAround->SetClockwise(true);

		auto rollingFT = rolling->AddComponent<FighterComponent>();
		rollingFT->SetAtk(5);
		rollingFT->SetHp(50);

		auto rollingAtk = rolling->AddComponent<AttackOneTimeComponent>();

		auto rollingED = rolling->AddComponent<EnemyDamageComponent>();

		SquareMesh rollingMesh;
		auto rollingRender = rolling->AddComponent<Render3DComponent>();
		rollingRender->SetMesh(rollingMesh);
		rollingRender->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		rollingRender->SetTexture("assets/texture/sword.png");

		auto rollingCollRend = rolling->AddComponent<Render3DColliderOBBComponent>();
		rollingCollRend->SetMesh(rollingMesh);
		rollingCollRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		rollingCollRend->SetTexture("assets/texture/NoTexture.png");
		rollingCollRend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));


		//auto rolling2 = GameObjectManager::AddObject("rolling2", "Sword");

		//auto rollingTrans2 = rolling2->AddComponent<TransformComponent>();
		//rollingTrans2->SetScale({ 8.0f, 3.0f, 3.0f });
		//rollingTrans2->SetPosition({ -30.0f,-9.0f,0.0f });
		//rollingTrans2->SetRotation({ 0.0f, 0.0f, 0.0f });

		//auto rollingGoAround2 = rolling2->AddComponent<GoAroundComponent>();
		//rollingGoAround2->SetCenterObject(player); // プレイヤーを中心に回るように設定
		//rollingGoAround2->MakeInitialOffset(playerTrans->GetPosition(), rollingTrans->GetPosition()); // 初期オフセットを設定
		//rollingGoAround2->SetRotationSpeed(7.0f); // 回転速度を設定
		//rollingGoAround2->SetInitialAngle(180.0f); // 初期角度を設定

		//SquareMesh rollingMesh2;
		//auto rollingRender2 = rolling2->AddComponent<Render3DComponent>();
		//rollingRender2->SetMesh(rollingMesh2);
		//rollingRender2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		//rollingRender2->SetTexture("assets/texture/NoTexture.png");

		// == ここ大事↑ ==


		/*CubeMesh cubeMeshSword;
		CubeMesh cubeMeshSword2;
		auto sword = GameObjectManager::AddChild("sword", "Cube");

		player->SetChild(sword);

		auto swordTrans = sword->AddComponent<TransformComponent>();
		swordTrans->SetLocalScale({ 1.5f, 0.5f, 1.0f });
		swordTrans->SetLocalPosition({ 3.0f, 2.0f, 0.0f });
		swordTrans->SetLocalRotation({ 0.0f, 0.0f, 30.0f });
		swordTrans->MakeChildWorld();

		auto swordColl = sword->AddComponent<ColliderComponent>();
		swordColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));
		swordColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto swordAttack = sword->AddComponent<AttackComponent>();
		swordAttack->SetCoolDownTime(1.0f);

		auto fighter = sword->AddComponent<FighterComponent>();
		fighter->SetHp(100);
		fighter->SetAtk(10);

		sword->AddComponent<EnemyDamageComponent>();

		auto swordRe = sword->AddComponent<Render3DComponent>();
		swordRe->SetMesh(cubeMeshSword);
		swordRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		swordRe->SetTexture("assets/texture/NoTexture.png");

		auto swordRe2 = sword->AddComponent<Render3DColliderOBBComponent>();
		swordRe2->SetMesh(cubeMeshSword2);
		swordRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		swordRe2->SetTexture("assets/texture/NoTexture.png");
		swordRe2->SetColor(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 0.5f));*/
	}

	TargetAndScroolCreate();

	{
		/*auto testUI = GameObjectManager::AddUI("TestUI", "TestUI");
		auto testUITrans = testUI->AddComponent<TransformComponent>();
		testUITrans->SetPosition({ -300.0f, 0.0f, 0.0f });
		testUITrans->SetScale({ 50.0f,50.0f,50.0f });
		auto testUIRend = testUI->AddComponent<Render2DComponent>();
		SquareMesh square;
		testUIRend->SetMesh(square);
		testUIRend->SetTexture("assets/texture/3count.png");
		testUIRend->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		testUIRend->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		auto uiTex = testUIRend->GetTexture();
		uiTex->SetInitialCut(2.0f, 2.0f);*/
	}

	auto hp = GameObjectManager::AddUI("hpUI", "HP_UI");
	auto hpTrans = hp->AddComponent<TransformComponent>();
	hpTrans->SetPosition({ -570.0f, 70.0f, 0.0f });
	hpTrans->SetScale({ 30.0f, 1.0f, 1.0f });

	auto hpBar = hp->AddComponent<HPBarMoveComponent>();

	hpBar->SetReferenceHPObj(*playOBJ);

	SquareMesh squareMesh;
	auto hpRender = hp->AddComponent<Render3DComponent>();
	hpRender->SetMesh(squareMesh);
	hpRender->SetShader("OverVertexMoveVS.hlsl", "shader/unlitTexturePS.hlsl");
	hpRender->SetTexture("assets/texture/NoTexture.png");
	hpRender->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	auto fade = GameObjectManager::GameObjectFindTagUI("FadeUI");
	if (!fade.empty()) {
		auto fadeUI = fade[0]->GetComponent<DoorFadeComponent>();
		fadeUI->SetBootDoor(true);
	//	fadeUI->SetNextSceneName();
	}
	else {
		auto fadeUI = GameObjectManager::AddUI("fade", "FadeUI");
		fadeUI->SetCarryOverFlag(true);
		auto fadeTrans = fadeUI->AddComponent<TransformComponent>();
		auto fadeAC = fadeUI->AddComponent<DoorFadeComponent>();
		fadeAC->SetNextSceneName("LoadStageScene");
	}
}

LoadStageScene::~LoadStageScene() {
//	GameObjectManager::ListClear(); // ゲームオブジェクトのリストをクリア
	GameObjectManager::OtherThanClear(); // 指定したタグ以外のゲームオブジェクトのリストをクリア
}

void LoadStageScene::Update() {

}

void LoadStageScene::TargetAndScroolCreate() {

	{
		auto target1 = GameObjectManager::AddObject("target1", "Target");
		auto targetTrans1 = target1->AddComponent<TransformComponent>();
		targetTrans1->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans1->SetPosition({ 10.0f, 30.0f, 0.0f });
		auto rigidTa1 = target1->AddComponent<RigidBodyComponent>();
		rigidTa1->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget1 = target1->AddComponent<CameraTargetComponent>();
		cameratarget1->SetCameraPattern(SPRING_CHASE);
		cameratarget1->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend1 = target1->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh1;
		targetRend1->SetMesh(sphereMesh1);
		targetRend1->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend1->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend1->SetTexture("assets/texture/NoTexture.png");


		auto target2 = GameObjectManager::AddObject("target2", "Target");
		auto targetTrans2 = target2->AddComponent<TransformComponent>();
		targetTrans2->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans2->SetPosition({ 240.0f, 30.0f, 0.0f });
		auto rigidTa2 = target2->AddComponent<RigidBodyComponent>();
		rigidTa2->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget2 = target2->AddComponent<CameraTargetComponent>();
		cameratarget2->SetCameraPattern(SPRING_CHASE);
		cameratarget2->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend2 = target2->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh2;
		targetRend2->SetMesh(sphereMesh2);
		targetRend2->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend2->SetTexture("assets/texture/NoTexture.png");

		auto target3 = GameObjectManager::AddObject("target3", "Target");
		auto targetTrans3 = target3->AddComponent<TransformComponent>();
		targetTrans3->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans3->SetPosition({ 240.0f, -120.0f, 0.0f });
		auto rigidTa3 = target3->AddComponent<RigidBodyComponent>();
		rigidTa3->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget3 = target3->AddComponent<CameraTargetComponent>();
		cameratarget3->SetCameraPattern(SPRING_CHASE);
		cameratarget3->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend3 = target3->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh3;
		targetRend3->SetMesh(sphereMesh3);
		targetRend3->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend3->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend3->SetTexture("assets/texture/NoTexture.png");

		auto target4 = GameObjectManager::AddObject("target4", "Target");
		auto targetTrans4 = target4->AddComponent<TransformComponent>();
		targetTrans4->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans4->SetPosition({ 240.0f, -290.0f, 0.0f });
		auto rigidTa4 = target4->AddComponent<RigidBodyComponent>();
		rigidTa4->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget4 = target4->AddComponent<CameraTargetComponent>();
		cameratarget4->SetCameraPattern(SPRING_CHASE);
		cameratarget4->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend4 = target4->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh4;
		targetRend4->SetMesh(sphereMesh4);
		targetRend4->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend4->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend4->SetTexture("assets/texture/NoTexture.png");

		auto target5 = GameObjectManager::AddObject("target5", "Target");
		auto targetTrans5 = target5->AddComponent<TransformComponent>();
		targetTrans5->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans5->SetPosition({ 450.0f, -290.0f, 0.0f });
		auto rigidTa5 = target5->AddComponent<RigidBodyComponent>();
		rigidTa5->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget5 = target5->AddComponent<CameraTargetComponent>();
		cameratarget5->SetCameraPattern(SPRING_CHASE);
		cameratarget5->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend5 = target5->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh5;
		targetRend5->SetMesh(sphereMesh5);
		targetRend5->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend5->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend5->SetTexture("assets/texture/NoTexture.png");

		auto target6 = GameObjectManager::AddObject("target6", "Target");
		auto targetTrans6 = target6->AddComponent<TransformComponent>();
		targetTrans6->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans6->SetPosition({ 700.0f, -290.0f, 0.0f });
		auto rigidTa6 = target6->AddComponent<RigidBodyComponent>();
		rigidTa6->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget6 = target6->AddComponent<CameraTargetComponent>();
		cameratarget6->SetCameraPattern(SPRING_CHASE);
		cameratarget6->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend6 = target6->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh6;
		targetRend6->SetMesh(sphereMesh6);
		targetRend6->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend6->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend6->SetTexture("assets/texture/NoTexture.png");

		auto target7 = GameObjectManager::AddObject("target7", "Target");
		auto targetTrans7 = target7->AddComponent<TransformComponent>();
		targetTrans7->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans7->SetPosition({ 950.0f, -290.0f, 0.0f });
		auto rigidTa7 = target7->AddComponent<RigidBodyComponent>();
		rigidTa7->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget7 = target7->AddComponent<CameraTargetComponent>();
		cameratarget7->SetCameraPattern(SPRING_CHASE);
		cameratarget7->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend7 = target7->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh7;
		targetRend7->SetMesh(sphereMesh7);
		targetRend7->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend7->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend7->SetTexture("assets/texture/NoTexture.png");

		auto target8 = GameObjectManager::AddObject("target8", "Target");
		auto targetTrans8 = target8->AddComponent<TransformComponent>();
		targetTrans8->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans8->SetPosition({ 1200.0f, -290.0f, 0.0f });
		auto rigidTa8 = target8->AddComponent<RigidBodyComponent>();
		rigidTa8->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget8 = target8->AddComponent<CameraTargetComponent>();
		cameratarget8->SetCameraPattern(SPRING_CHASE);
		cameratarget8->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend8 = target8->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh8;
		targetRend8->SetMesh(sphereMesh8);
		targetRend8->SetColor({ 1.0f,0.0f,0.0f,0.0f });
		targetRend8->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend8->SetTexture("assets/texture/NoTexture.png");

		//auto circle = GameObjectManager::AddObject("Scroll", "Scroll");
		//auto circleTrans = circle->AddComponent<TransformComponent>();
		//circleTrans->SetScale({ 30.0f, 30.0f, 10.0f });
		//circleTrans->SetPosition({ -80.0f, 60.0f, 0.0f });
		//auto rigidMe = circle->AddComponent<RigidBodyComponent>();
		//rigidMe->SetActiveFlag(true); // 物理演算を有効にする
		//auto circleRend = circle->AddComponent<Render3DComponent>();
		//CircleMesh circleMesh;
		//circleRend->SetMesh(circleMesh);
		//circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		//circleRend->SetTexture("assets/texture/NoTexture.png");

		//auto circleSpring = circle->AddComponent<SpringComponent>();
		//circleSpring->SetK(20.0f); // ばね定数をセット
		//circleSpring->MakeDamping(); // ダンピングを作成
		//circleSpring->SetSpringPartner(target);

		//cameraSpring->SetK(20.0f); // ばね定数をセット
		//cameraSpring->MakeDamping(); // ダンピングを作成
		//cameraSpring->SetSpringPartner(target);

		auto point1 = GameObjectManager::AddObject("CameraPoint1", "CameraPoint");
		auto pointTrans1 = point1->AddComponent<TransformComponent>();
		pointTrans1->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans1->AddPosition({ 120.0f, 50.0f, 0.0f });
		auto pointCamera1 = point1->AddComponent<CameraPointComponent>();
		auto pointColl1 = point1->AddComponent<ColliderComponent>();
		CubeMesh pointMesh1;
		auto pointRend1 = point1->AddComponent<Render3DColliderAABBComponent>();
		pointRend1->SetMesh(pointMesh1);
		pointRend1->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend1->SetTexture("assets/texture/NoTexture.png");
		pointRend1->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera1->SetBeforeAndNextTargetObj(*target1, *target2);
		pointCamera1->SetScrollDirection(SCROLL_IN_LEFT);

		auto point2 = GameObjectManager::AddObject("CameraPoint2", "CameraPoint");
		auto pointTrans2 = point2->AddComponent<TransformComponent>();
		pointTrans2->SetScale({ 30.0f, 5.0f, 10.0f });
		pointTrans2->AddPosition({ 305.0f, -30.0f, 0.0f });
		auto pointCamera2 = point2->AddComponent<CameraPointComponent>();
		auto pointColl2 = point2->AddComponent<ColliderComponent>();
		CubeMesh pointMesh2;
		auto pointRend2 = point2->AddComponent<Render3DColliderAABBComponent>();
		pointRend2->SetMesh(pointMesh2);
		pointRend2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend2->SetTexture("assets/texture/NoTexture.png");
		pointRend2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera2->SetBeforeAndNextTargetObj(*target2, *target3);
		pointCamera2->SetScrollDirection(SCROLL_IN_UP);

		auto point3 = GameObjectManager::AddObject("CameraPoint3", "CameraPoint");
		auto pointTrans3 = point3->AddComponent<TransformComponent>();
		pointTrans3->SetScale({ 30.0f, 6.0f, 10.0f });
		pointTrans3->AddPosition({ 160.0f, -180.0f, 0.0f });
		auto pointCamera3 = point3->AddComponent<CameraPointComponent>();
		auto pointColl3 = point3->AddComponent<ColliderComponent>();
		CubeMesh pointMesh3;
		auto pointRend3 = point3->AddComponent<Render3DColliderAABBComponent>();
		pointRend3->SetMesh(pointMesh3);
		pointRend3->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend3->SetTexture("assets/texture/NoTexture.png");
		pointRend3->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera3->SetBeforeAndNextTargetObj(*target3, *target4);
		pointCamera3->SetScrollDirection(SCROLL_IN_UP);

		auto point4 = GameObjectManager::AddObject("CameraPoint4", "CameraPoint");
		auto pointTrans4 = point4->AddComponent<TransformComponent>();
		pointTrans4->SetScale({ 10.0f, 90.0f, 10.0f });
		pointTrans4->AddPosition({ 330.0f, -260.0f, 0.0f });
		auto pointCamera4 = point4->AddComponent<CameraPointComponent>();
		auto pointColl4 = point4->AddComponent<ColliderComponent>();
		CubeMesh pointMesh4;
		auto pointRend4 = point4->AddComponent<Render3DColliderAABBComponent>();
		pointRend4->SetMesh(pointMesh4);
		pointRend4->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend4->SetTexture("assets/texture/NoTexture.png");
		pointRend4->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera4->SetBeforeAndNextTargetObj(*target4, *target5);
		pointCamera4->SetScrollDirection(SCROLL_IN_LEFT);

		auto point5 = GameObjectManager::AddObject("CameraPoint5", "CameraPoint");
		auto pointTrans5 = point5->AddComponent<TransformComponent>();
		pointTrans5->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans5->AddPosition({ 580.0f, -280.0f, 0.0f });
		auto pointCamera5 = point5->AddComponent<CameraPointComponent>();
		auto pointColl5 = point5->AddComponent<ColliderComponent>();
		CubeMesh pointMesh5;
		auto pointRend5 = point5->AddComponent<Render3DColliderAABBComponent>();
		pointRend5->SetMesh(pointMesh5);
		pointRend5->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend5->SetTexture("assets/texture/NoTexture.png");
		pointRend5->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera5->SetBeforeAndNextTargetObj(*target5, *target6);
		pointCamera5->SetScrollDirection(SCROLL_IN_LEFT);

		auto point6 = GameObjectManager::AddObject("CameraPoint6", "CameraPoint");
		auto pointTrans6 = point6->AddComponent<TransformComponent>();
		pointTrans6->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans6->AddPosition({ 830.0f, -280.0f, 0.0f });
		auto pointCamera6 = point6->AddComponent<CameraPointComponent>();
		auto pointColl6 = point6->AddComponent<ColliderComponent>();
		CubeMesh pointMesh6;
		auto pointRend6 = point6->AddComponent<Render3DColliderAABBComponent>();
		pointRend6->SetMesh(pointMesh6);
		pointRend6->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend6->SetTexture("assets/texture/NoTexture.png");
		pointRend6->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera6->SetBeforeAndNextTargetObj(*target6, *target7);
		pointCamera6->SetScrollDirection(SCROLL_IN_LEFT);

		auto point7 = GameObjectManager::AddObject("CameraPoint7", "CameraPoint");
		auto pointTrans7 = point7->AddComponent<TransformComponent>();
		pointTrans7->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans7->AddPosition({ 1080.0f, -280.0f, 0.0f });
		auto pointCamera7 = point7->AddComponent<CameraPointComponent>();
		auto pointColl7 = point7->AddComponent<ColliderComponent>();
		auto bossEvent = point7->AddComponent<BossEventComponent>();

		CubeMesh pointMesh7;
		auto pointRend7 = point7->AddComponent<Render3DColliderAABBComponent>();
		pointRend7->SetMesh(pointMesh7);
		pointRend7->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend7->SetTexture("assets/texture/NoTexture.png");
		pointRend7->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera7->SetBeforeAndNextTargetObj(*target7, *target8);
		pointCamera7->SetScrollDirection(SCROLL_IN_LEFT);
	}
}